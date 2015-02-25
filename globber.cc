#include <sys/stat.h>
#include <assert.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include "globber.hpp"

Globber::Globber(const char* pattern) {
  matches = new Queue();
  glob    = new Path(pattern);

  // If pattern is an absolute path, the root should be '/'
  // otherwise, the root is current directory.
  root    = glob -> isAbsolute() ? new Path("/") : Globber::cwd();
  // Push root directory into the queue
  matches -> push(new Path());
}

Globber::~Globber() {
  while (!matches -> empty()) {
    Path *p = matches -> front();
    matches -> pop();
    delete p;
  }
  delete matches;
  delete root;
  delete glob;
}

void
Globber::run() {
  char *segment;
  while ((segment = glob -> popd())) {
    DBG_INFO("Globber::run(): segment: \"%s\"\n", segment);
    glob_segment(segment);
    free(segment);
  }
}

void
Globber::glob_segment(char* segment) {
  Queue *iteration = new Queue();

  // If the segment is a glob pattern, convert to regex and match it
  if (strstr(segment, "*") || strstr(segment, "?")) {

    // Convert the glob segment into regex
    Regex *regex = Globber::toRegex(segment);

    // For each match out there...
    while (!matches -> empty()) {
      Path *match = matches -> front();
      matches -> pop();
      glob_regex(match, regex, iteration);
      delete match;
    }

    free(regex);
  }

  // Otherwise, the segment is simply a directory which we push in
  else {
    while (!matches -> empty()) {
      Path *match = matches -> front();
      matches -> pop();
      glob_single(match, segment, iteration);
    }
  }

  assert(matches -> empty());
  delete matches;
  matches = iteration;
}

void
Globber::glob_single(Path *match, char *segment, Queue *iteration) {
  // Push in current glob segment
  match -> pushd(segment);

  // Compute the absolute path of the match
  char *full_path  = match -> resolve(root);

  // Check whether the match exists
  DirStat d_stat;
  if (stat(full_path, &d_stat) ||
     (!glob -> count() && !S_ISDIR(d_stat.st_mode))) {
    free(full_path);
    return;
  }
  free(full_path);

  // Push the match into iteration queue
  iteration -> push(match -> print());
}

void
Globber::glob_regex(Path *match, Regex *regex, Queue *iteration) {
  // Compute full path of this match
  char *full_match = match -> resolve(root);

  // Open the matching directory for file listing
  // Bail if cannot open the directory
  DIR      *dir;
  DirEntry *ent;
  if (!(dir = opendir(full_match))) {
    DBG_ERR("%s: %s\n", full_match, strerror(errno));
    free(full_match);
    return;
  }
  free(full_match);

  // For each file entry in the match
  while ((ent = readdir(dir))) {

    // Skip '.' and '..' entries
    if (!strcmp(ent->d_name, "." )) { continue; }
    if (!strcmp(ent->d_name, "..")) { continue; }

    // Skip if regex does not match
    regmatch_t m;
    if (regexec(regex, ent->d_name, 1, &m, 0)) { continue; }

    // Compute the full path of the sub-match
    Path *submatch = match -> clone() -> pushd(ent -> d_name) -> print();
    char *sub_path = submatch -> resolve(root);

    // Get stats to determine whether this is a directory
    DirStat d_stat;
    if (stat(sub_path, &d_stat)) {
      DBG_ERR("Path::glob(): %s: %s: %s\n", YELLOW("stat"), sub_path, strerror(errno));
      free(submatch);
      free(sub_path);
      continue;
    }
    free(sub_path);

    // If this is a file, and we still have unprocessed segments, no match
    if (glob -> count() && !S_ISDIR(d_stat.st_mode)) {
      free(submatch);
      continue;
    }

    // Push the match into iteration queue
    iteration -> push(submatch);
  }
}

Path*
Globber::cwd() {
  char tmp[2048];
  memset(tmp, sizeof(char) * 2048, 0);
  if (getcwd(tmp, sizeof(tmp))) {
    return new Path(tmp);
  }
  return NULL;
}

Regex*
Globber::toRegex(const char *glob) {
  if (!glob) return NULL;
  std::string *result = new std::string(glob);

  Path::replace(result, ".", "\\.");
  int pos  = 0;
  while ((pos = result ->find("*", pos)) != std::string::npos) {
    if (pos != 0) {
      result -> replace(pos, 1, ".*");
      pos += 2;
    }
    else {
      result -> replace(pos, 1, "[^.].*");
      pos += 6;
    }
  }
  pos = 0;
  while ((pos = result ->find("?", pos)) != std::string::npos) {
    if (pos != 0) {
      result -> replace(pos, 1, ".");
      pos += 1;
    }
    else {
      result -> replace(pos, 1, "[^.]");
      pos += 4;
    }
  }

  result -> insert(0, "^");
  result -> push_back('$');

  Regex *regex = (Regex*)malloc(sizeof(Regex));

  int      fail  = regcomp(regex, result -> c_str(), REG_EXTENDED|REG_NOSUB);
  if (fail) {
    COMPLAIN("%s: %s", result -> c_str(), strerror(errno));
    delete result;
    return NULL;
  }

  delete result;
  return regex;
}
