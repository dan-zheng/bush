#include <stdlib.h>
#include <string.h>
#include <string>

#include "path.h"


char*
Path::join(char *path, char *dir) {

  char* result = strdup(path);
  char* onedir;

  while ((onedir = Path::popd(&dir))) {
    Path::pushd(&result, onedir);
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

void
Path::pushd(char **path, char *dir) {

  // Do not handle null arguments
  if (!path || !*path || !dir) return;

  // There are three cases for dir pushes:
  if (!strcmp(dir, ".")) {
    // Current directory, there is nothing to push.
    return;
  }
  else if (!strcmp(dir, "..")) {
    // Parent directory, remove the last directory name
    char *ps = *path + strlen(*path);
    while (ps > *path && *ps != '/') { ps--; }
    *ps = 0;
  }
  else {
    // Child directory, push it into the path
    // Compute lengths of both components
    int  path_length = strlen(*path);
    int  dir_length  = strlen(dir);

    // Remove tailing slash from path if needed
    if (*(*path + path_length - 1) == '/') {
      *(*path + path_length - 1) = 0;
      path_length -= 1;
    }

    // Remove heading slash from dir if needed
    if (*dir == '/') {
      dir += 1;
      dir_length -= 1;
    }

    // Re-allocate the path string and concat components
    char *ns = (char*) malloc(sizeof(char) * (path_length + dir_length + 1));
    strcpy(ns, *path);
    *(ns + path_length) = '/';
    strcpy(ns + path_length + 1, dir);

    // Free old path and assign new path
    free(*path);
    *path = ns;
  }

  DBG_VERBOSE("Path::pushdir(): path: \"%s\"\n", *path);
}


void
Path::glob2rgx(char **glob) {
  if (!glob || !*glob) return;
  std::string *result = new std::string(*glob);

  int pos = 0;
  while ((pos = result->find('.', pos)) != std::string::npos) {
    result -> replace(pos, 1, "\\.");
    pos += 2;
  }
  pos = 0;
  while ((pos = result->find('*', pos)) != std::string::npos) {
    result -> replace(pos, 1, ".*");
    pos += 2;
  }
  pos = 0;
  while ((pos = result->find('?', pos)) != std::string::npos) {
    result -> replace(pos, 1, ".");
    pos += 1;
  }

  free(*glob);
  *glob = strdup(result -> c_str());
  delete result;
}
