#include <stdio.h>
#include <errno.h>
#include <regex.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
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

  // Find the first path delimiter (or end of string)
  char *pe  = strstr(*path, "/");
  if (!pe) { pe = *path + strlen(*path); }

  // Do not handle strings with 0 length
  if (pe == *path) return NULL;

  // Duplicate the first directory name, and remove it from path

  char *dir  = strndup(*path, pe - *path);
  char *np   = strdup(pe + 1);

  free(*path);
  *path = np;

  DBG_VERBOSE("Path::popdir(): dir: \"%s\"\n", dir);
  DBG_VERBOSE("Path::popdir(): path: \"%s\"\n", *path);
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

  }
  else if (!strcmp(dir, "..")) {
    // Parent directory, remove the last directory name
    char *ps = path + strlen(path);
    while (ps > path && *ps != '/') { ps--; }
    *ps = 0;
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
    char *ns = (char*) malloc(sizeof(char) * (path_length + dir_length + 1));
    strcpy(ns, path);
    *(ns + path_length) = '/';
    strcpy(ns + path_length + 1, dir);

    free(result);
    result = ns;
  }
  DBG_VERBOSE("Path::pushdir(): dir: \"%s\"\n", dir);
  DBG_VERBOSE("Path::pushdir(): path: \"%s\"\n", result);
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

Queue*
Path::glob(char* pattern) {
  Queue *result    = new Queue();
  Queue *iteration = new Queue();

  // Push in the current working dir
  result -> push(Path::cwd());


  char *glob    = strdup(pattern);
  char *partial;
  while ((partial = Path::popd(&glob))) {

    // Only perform regex glob if partial contains wildcards
    if (strstr(partial, "*") || strstr(partial, "?")) {

      // Compile pattern fragment into regex
      regex_t *regex = Path::glob2rgx(partial);
      free(partial);

      // For each previously matched path
      while (!result -> empty()) {
        char *candidate = result -> front();
        result -> pop();

        DIR      *dir;
        DirEntry *ent;
        if ((dir = opendir(candidate))) {
          // Iterate through all entries
          while ((ent = readdir(dir))) {

            // Skip '.' and '..'
            if (!strcmp(ent->d_name, ".") ||
                !strcmp(ent->d_name, "..")) {
              continue;
            }

            // Skip files if we still have un-popped partials
            if (strlen(glob) != 0 && ent->d_type != DT_DIR) {
              continue;
            }

            // Push matches into result
            regmatch_t m;
            if (!regexec(regex, ent->d_name, 1, &m, 0)) {
              char* full = Path::pushd(candidate, ent->d_name);
              iteration -> push(full);
              DBG_INFO("Path::glob(): regex: %s\n", full);
            }

          }
        }
        else {
          COMPLAIN("%s: %s", candidate, strerror(errno));
          // Clean up
          free(candidate);
          free(regex);
          while (!result -> empty()) {
            free(result->front());
            result->pop();
          }
          while (!iteration -> empty()) {
            free(iteration->front());
            iteration->pop();
          }
          delete result;
          delete iteration;
          return NULL;
        }

        free(candidate);
      }

      // Clean up and go to next iteration
      free(regex);

    }
    // No wildcards, perform directo pushd and copy
    else {

      while (!result -> empty()) {
        char *candidate = result -> front();
        result -> pop();

        char *full = Path::pushd(candidate, partial);
        free(candidate);

        // Check for existence
        if (access(full, F_OK) != -1) {
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
    iteration = new Queue();
  }

  return result;
}
