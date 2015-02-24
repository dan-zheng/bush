#include <stdio.h>
#include <errno.h>
#include <regex.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string>
#include <queue>

#include "path.h"
#include "util.h"


char*
Path::cwd() {
  char tmp[2048];
  if (getcwd(tmp, sizeof(tmp))) {
    return strdup(tmp);
  }
  return NULL;
}


char*
Path::join(char *path, char *dir) {

  char* result = strdup(path);
  char* onedir;

  while ((onedir = Path::popd(&dir))) {
    char *t = Path::pushd(result, onedir);
    free(result);
    result = t;
    free(onedir);
  }

  return result;
}

char*
Path::popd(char **path) {

  // Do not handle NULL paths
  if (!path || !*path) return NULL;
  if (strlen(*path) == 0) return NULL;

  // Find the first path delimiter (or end of string)
  char *pe  = strstr(*path, "/"),
       *np,
       *dir;

  // We do have a separator, so pop first dir
  if (pe) {
    // Duplicate the first directory name, and remove it from path
    dir = strndup(*path, pe - *path);
    np  = strdup(pe + 1);
  }
  // We do not have a separator, pop the entire thing
  else {
    dir = strdup(*path);
    np  = (char*)malloc(sizeof(char));
    *np = 0;
  }

  free(*path);
  *path = np;
  return dir;
}

char*
Path::pushd(char *path, char *dir) {

  // Do not handle null arguments
  if (!path || !dir) return NULL;

  char *result = strdup(path);

  // There are three cases for dir pushes:
  if (!strcmp(dir, ".")) {
    // Current directory, there is nothing to push.
    return strdup(path);
  }
  else if (!strcmp(dir, "..")) {
    // Parent directory, remove the last directory name
    char *np = strdup(path);
    char *ps = np + strlen(path);
    while (ps > np && *ps != '/') { ps--; }
    *ps = 0;
    DBG_VERBOSE("Path::pushd(): %s\n", np);
    return np;
  }
  else {
    // Child directory, push it into the path
    // Compute lengths of both components
    int  path_length = strlen(path);
    int  dir_length  = strlen(dir);

    // Remove tailing slash from path if needed
    if (*(path + path_length - 1) == '/') {
      *(path + path_length - 1) = 0;
      path_length -= 1;
    }

    // Remove heading slash from dir if needed
    if (*dir == '/') {
      dir += 1;
      dir_length -= 1;
    }

    // Re-allocate the path string and concat components
    char *ns = (char*) malloc(sizeof(char) * (path_length + dir_length + 2));
    strcpy(ns, path);
    *(ns + path_length) = '/';
    strcpy(ns + path_length + 1, dir);

    free(result);
    result = ns;
  }
  return result;
}


regex_t*
Path::glob2rgx(char *glob) {
  if (!glob) return NULL;
  std::string *result = new std::string(glob);

  Util::replace(result, ".", "\\.");
  Util::replace(result, "*", ".*");
  Util::replace(result, "?", ".");
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

char**
Path::glob(char* pattern) {
  Queue *result    = new Queue();
  Queue *iteration;

  // Push in the current working dir
  result -> push(Path::cwd());

  char *glob    = strdup(pattern);
  char *partial;
  while ((partial = Path::popd(&glob))) {

    iteration = new Queue();

    DBG_VERBOSE("Path::glob(): %s: \"%s\"\n", "partial", partial);

    // Only perform regex glob if partial contains wildcards
    if (strstr(partial, "*") || strstr(partial, "?")) {

      DBG_VERBOSE("Path::glob(): Regex handler.\n");

      // Compile pattern fragment into regex
      regex_t *regex = Path::glob2rgx(partial);
      free(partial);

      // For each previously matched path
      while (!result -> empty()) {
        char *candidate = result -> front();
        result -> pop();

        // Open directory for reading
        DIR      *dir;
        DirEntry *ent;
        if (!(dir = opendir(candidate))) {
          DBG_ERR("%s: %s", candidate, strerror(errno));
          free(candidate);
          continue;
        }

        // Iterate through all sub-entries
        while ((ent = readdir(dir))) {

          // Skip '.' and '..'
          if (!strcmp(ent->d_name, ".") ||
              !strcmp(ent->d_name, "..")) {
            continue;
          }

          // Check if regex matches
          regmatch_t m;
          if (regexec(regex, ent->d_name, 1, &m, 0)) { continue; }

          // Compute the full path of the file in question
          char* full = Path::pushd(candidate, ent->d_name);

          // Get entry stats to determine its type
          DirStat f_stat;
          if (stat(full, &f_stat)) {
            DBG_ERR("Path::glob(): %s: %s\n", YELLOW("stat"), strerror(errno));
            free(full);
            continue;
          }

          // If this is not a DT_DIR, and we still have glob partials to go,
          // this match needs to be skipped
          if (strlen(glob) != 0 && !S_ISDIR(f_stat.st_mode)) {
            DBG_VERBOSE("Path::glob(): %s: %d \"%s\"\n", RED("skip"), ent->d_type, ent->d_name);
            free(full);
            continue;
          }

          // Push it into the iteration queue
          iteration -> push(full);
          DBG_INFO("Path::glob(): regex: %s\n", full);
        }

        // We have no more use for *candidate, free it
        free(candidate);
      }

      // Clean up and go to next iteration
      free(regex);
    }
    // No wildcards, perform direct pushd and copy
    else {

      while (!result -> empty()) {
        char *candidate = result -> front();
        result -> pop();

        // No wildcards means pushd the partial into candidate
        // and we're good to go.
        char *full = Path::pushd(candidate, partial);
        free(candidate);

        // Check whether the file exists
        DirStat f_stat;
        if (!stat(full, &f_stat)) {
          iteration -> push(full);
          DBG_INFO("Path::glob(): pushd: %s\n", full);
        } else {
          free(full);
        }
      }

      // Clean up and go to next iteration
      free(partial);
    }

    delete result;
    result    = iteration;
  }

  // Convert to null-terminated array
  char **array = (char**)malloc(sizeof(char*) * (result->size() + 1));
  char **pw    = array;
  while (!result -> empty()) {
    *pw++ = result -> front();
    result -> pop();
  }
  *pw = 0;

  // Clean up the stuff and return
  free(glob);
  delete result;
  return array;
}
