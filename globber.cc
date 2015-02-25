#include <stdio.h>
#include <errno.h>
#include <regex.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string>
#include <queue>

#include "util.h"
#include "globber.h"


Globber::Globber(char *pattern) {
  matches = new Queue();
  root    = (*pattern == '/') ? strdup("/") : Globber::cwd();
  glob    = strdup(pattern);

  matches -> push(strdup(root));
}

Globber::~Globber() {
  delete matches;
  free(root);
  free(glob);
}

void
Globber::next() {
  char *segment;
  while ((segment = Globber::popd(glob))) {
    next_segment(segment);
    free(segment);
  }
}

void
Globber::next_segment(char *segment) {
  Queue *results = new Queue();

  // Check if the segment contains wildcards
  // if it does, go through all that regex fanciness
  if (strstr(segment, "*") || strstr(segment, "?")) {
    // Convert segment to regex
    regex_t *regex = Globber::torgx(segment);

    // For each match from previous round
    while (!matches -> empty()) {
      char *match = matches -> front();
      matches -> pop();
      next_match(match, regex, results);
      free(match);
    }

    free(regex);
  }
  // ..if it doesn't, just simply push it to all matches
  else {
    // For each match...
    while (!matches -> empty()) {
      char *match = matches -> front();
      matches -> pop();

      // No wildcards means simply pushd the current segment
      char *filename = Globber::pushd(match, segment);

      // Check whether the file exists
      // Also skip if this is a file, and we still have upcoming segments
      DirStat d_stat;
      if (stat(filename, &d_stat) || (strlen(glob) && !S_ISDIR(d_stat.st_mode))) {
        free(match);
        free(filename);
        continue;
      }

      // Push filename into results
      results -> push(filename);

      // Cleanup stuff that we no longer need
      free(match);
    }
  }

  // Clear previous matches and push in new matches
  assert(matches -> empty());
  delete matches;
  matches = results;
}

void
Globber::next_match(char *match, regex_t *regex, Queue *results) {
  // Open the matching directory for file listing
  DIR      *dir;
  DirEntry *ent;
  // Bail if cannot open the directory
  if (!(dir = opendir(match))) {
    DBG_ERR("%s: %s\n", match, strerror(errno));
    return;
  }

  // For each file entry in the match
  while ((ent = readdir(dir))) {

    // Skip '.' and '..' entries
    if (!strcmp(ent->d_name, "." )) { continue; }
    if (!strcmp(ent->d_name, "..")) { continue; }

    // Skip if regex does not match
    regmatch_t m;
    if (regexec(regex, ent->d_name, 1, &m, 0)) { continue; }

    // Compute the file name and full file path
    char *filename = Globber::pushd(match, ent -> d_name);

    DBG_VERBOSE("Globber::next_match(): push: \"%s\"\n", filename);
    // Get stats to determine whether this is a directory
    DirStat d_stat;
    if (stat(filename, &d_stat)) {
      DBG_ERR("Path::glob(): %s: %s: %s\n", YELLOW("stat"), filename, strerror(errno));
      free(filename);
      continue;
    }

    // If this is a file, and we still have unprocessed segments, no match
    if (strlen(glob) && !S_ISDIR(d_stat.st_mode)) {
      DBG_VERBOSE("Path::glob(): %s: %d \"%s\"\n", RED("skip"), ent->d_type, ent->d_name);
      free(filename);
      continue;
    }

    // Put the filename into results queue
    results -> push(filename);
  }
}

// ------------------------------------------------------------------------- //
// STATIC: Gets the current directory of the shell.                          //
// ------------------------------------------------------------------------- //
char*
Globber::cwd() {
  char tmp[2048];
  if (getcwd(tmp, sizeof(tmp))) {
    return strdup(tmp);
  }
  return NULL;
}

char*
Globber::join(const char *path, const char *dir) {
  char   *onedir,
         *result = strdup(path),
         *tmpdir = strdup(dir);
  while ((onedir = Globber::popd(tmpdir))) {
    char *t      = Globber::pushd(result, onedir);
    free(onedir);
    free(result);
    result = t;
  }
  free(tmpdir);
  return result;
}

char*
Globber::popd(char *path) {
  // Do not handle NULL or empty paths
  if (!path) return NULL;
  if (strlen(path) == 0) return NULL;

  // Find the first path delimiter (or end of string)
  char *pe  = strstr(path, "/"),
       *np,
       *dir;

  // We do have a separator, so pop first dir
  if (pe) {
    // Duplicate the first directory name, and remove it from path
    dir = strndup(path, pe - path);
    np  = strdup(pe + 1);
  }
  // We do not have a separator, pop the entire thing
  else {
    dir = strdup(path);
    np  = strdup("");
  }

  memset(path, strlen(path) * sizeof(char), 0);
  strcpy(path, np);
  return dir;
}

char*
Globber::pushd(const char *path, const char *dir) {
  char *result = path ? strdup(path) : strdup("");
  char *tmpdir = dir ? strdup(dir) : strdup("");

  // There are three cases for dir pushes:
  if (!strcmp(tmpdir, ".")) {
    // Current directory, there is nothing to push.
    free(tmpdir);
    return result;
  }
  else if (!strcmp(tmpdir, "..") && strlen(result) > 0) {
    // Parent directory, remove the last directory name
    char *np = result;
    char *ps = np + strlen(result);
    while (ps > np && *ps != '/') { ps--; }
    *ps = 0;
    return np;
  }
  else {
    // Child directory, push it into the path
    // Compute lengths of both components
    int  path_length = strlen(result);
    int  dir_length  = strlen(tmpdir);

    // Remove tailing slash from path if needed
    if (*(result + path_length - 1) == '/') {
      *(result + path_length - 1) = 0;
      path_length -= 1;
    }

    // Remove heading slash from dir if needed
    if (*tmpdir == '/') {
      tmpdir += 1;
      dir_length -= 1;
    }

    // Re-allocate the path string and concat components
    char *ns = (char*) malloc(sizeof(char) * (path_length + dir_length + 2));
    strcpy(ns, result);
    *(ns + path_length) = '/';
    strcpy(ns + path_length + 1, tmpdir);

    free(tmpdir);
    free(result);
    result = ns;
  }
  return result;
}

regex_t*
Globber::torgx(const char *glob) {
  if (!glob) return NULL;
  std::string *result = new std::string(glob);

  Util::replace(result, ".", "\\.");
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

  regex_t *regex = (regex_t*)malloc(sizeof(regex_t));

  int      fail  = regcomp(regex, result -> c_str(), REG_EXTENDED|REG_NOSUB);
  if (fail) {
    COMPLAIN("%s: %s", result -> c_str(), strerror(errno));
    delete result;
    return NULL;
  }

  delete result;
  return regex;
}
