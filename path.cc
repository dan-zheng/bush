// ------------------------------------------------------------------------- //
//                                                                           //
// CS252 Lab03 - Shell                                                       //
// Copyright © 2015 Denis Luchkin-Zhou                                       //
//                                                                           //
// path.cc                                                                   //
// This file contains the logic for the class handling file path             //
// manipulation. This class forms the cornerstone of globbing.               //
//                                                                           //
// ------------------------------------------------------------------------- //

/*
 * Three Rings for the Elven-kings under the sky,
 * Seven for the Dwarf-lords in halls of stone,
 * Nine for Mortal Men, doomed to die,
 * One for the Dark Lord on his dark throne
 * In the Land of Mordor where the Shadows lie.
 * One Ring to rule them all, One Ring to find them,
 * One Ring to bring them all and in the darkness bind them.
 * In the Land of Mordor where the Shadows lie.
 *   -- J.R.R. Tolkien
 */

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include "trace.h"
#include "path.h"
#include "util.h"


// ------------------------------------------------------------------------- //
// Default constructor. Creates a relative Path object with no segments.     //
// ------------------------------------------------------------------------- //
Path::Path() { init(NULL); }

// ------------------------------------------------------------------------- //
// Constructor. Creates a path from its string representation.               //
// This constructor copies contents of the string, therefore the caller is   //
// responsible for freeing the memory.                                       //
// ------------------------------------------------------------------------- //
Path::Path(const char* str) { init(str); }

// ------------------------------------------------------------------------- //
// Destructor. Deletes all strings managed by this path, including segments. //
// ------------------------------------------------------------------------- //
Path::~Path() {
  // Delete all segments
  while (!segments -> empty()) {
    char *segment = segments -> front();
    segments -> pop_front();
    free(segment);
  }
}

// ------------------------------------------------------------------------- //
// Initializes the path instance from a string representation.               //
// If str is NULL, the instance will be an empty relative path.              //
// ------------------------------------------------------------------------- //
void
Path::init(const char* str) {
  const char *ps = str,
             *pe = str;
  // Initialize class members
  absolute = 0;
  segments = new SegmentList();
  // Watch out for null argument!
  if (!str) { return; }
  // Paths starting with '/' are absolute
  if (*ps == '/') {
    absolute = 1; ps += 1; pe = ps;
  }
  // Split paths into segments
  while (*pe++) {
    if (*pe == '/' && pe - ps > 1) {
      char *segment = strndup(ps, pe - ps);
      segments -> push_back(segment);
      ps = pe + 1;
      DBG_VERBOSE("Path::init(): segment: \"%s\"\n", segment);
    }
  }
  // Copy over the final segment
  if (pe - ps > 1) {
    char *segment = strdup(ps);
    segments -> push_back(segment);
    DBG_VERBOSE("Path::Path(): segment: \"%s\"\n", segment);
  }
}

// ------------------------------------------------------------------------- //
// Pushes a string path into the path object.                                //
// Resolves '.' and '..' segments where possible.                            //
// Copies the contents of the `path`, therefore the caller is responsible    //
// for freeing the memory.                                                   //
// ------------------------------------------------------------------------- //
void
Path::push(const char *path) {
  Path *p = new Path(path);
  push(p);
  delete p;
}

// ------------------------------------------------------------------------- //
// Pushes segments of another path into the path object.                     //
// Does not alter the `path` pbject.                                         //
// Copies segments of the `path`, therefore the caller is responsible        //
// for freeing the memory.                                                   //
// ------------------------------------------------------------------------- //
void
Path::push(Path *path) {
  SegmentList::iterator it = path -> segments -> begin();
  for (; it != path -> segments -> end(); ++it) { pushd(*it); }
}

// ------------------------------------------------------------------------- //
// Pushes one segment into the path.                                         //
// Technically, Path::push() is more flexible than this, but when you need   //
// to insert segments without resolving '.' and '..', try this one.          //
// Copies the `segment`, therefore the caller is responsible for freeing the //
// memory.                                                                   //
// ------------------------------------------------------------------------- //
void
Path::pushd(const char* segment) {
  if (!segment) { return; }

  // Get rid of heading and tailing slashes
  char *seg  = Util::trim(segment, '/');
  DBG_VERBOSE("Path::pushd(): segment: %s\n", seg);

  // If this is a '.', no need to push it at all!
  if (!strcmp(seg, ".")) { /* skip this one */ }
  // If this is a '..', pop a directory from back
  else if (!strcmp(seg, "..") && !segments -> empty()) {
    char *last = segments -> back();
    segments -> pop_back();
    free(last);
  }
  // So we either really need to push the segment, or we have
  // nothing left to pop.
  else {
    segments -> push_back(strdup(seg));
  }

  free(seg);
}

// ------------------------------------------------------------------------- //
// Pops the first segment og the path.                                       //
// Data returned by this method is no longer managed by the Path object,     //
// therefore it is up to the caller to free the return value.                //
// ------------------------------------------------------------------------- //
char*
Path::popd() {
  char *segment = segments -> front();
  segments -> pop_front();
  return segment;
}

// ------------------------------------------------------------------------- //
// Resolves unresolved '.' and '..' segments where possible.                 //
// ------------------------------------------------------------------------- //
void
Path::reduce() {
  SegmentList *backup   = segments;
               segments = new SegmentList();
  while (!backup -> empty()) {
    char *segment = backup -> front();
    backup -> pop_front();
    pushd(segment);
    free(segment);
  }
  delete backup;
}

// ------------------------------------------------------------------------- //
// Creates a string representation of the Path object.                       //
// Data returned by this method is not managed by the Path object, therefore //
// it is up to the caller to free the return value.                          //
// ------------------------------------------------------------------------- //
char*
Path::str() {
  char  buffer[PATH_MAX];
  char *pw = buffer;

  // Reset the buffer
  memset(buffer, sizeof(char) * PATH_MAX, 0);

  // New queue to insert items
  SegmentList *nseg     = segments;
               segments = new SegmentList();

  // Start absolute path with a slash
  if (absolute) { *pw++ = '/'; }

  while (!nseg -> empty()) {
    char *pr      = nseg -> front(),
         *segment = pr;

    nseg -> pop_front();
    while (*pr) { *pw++ = *pr++; }
    if (!nseg -> empty()) { *pw++ = '/'; }
    segments -> push_back(segment);
  }
  *pw = 0;
  assert(nseg -> empty());
  delete nseg;


  return strdup(buffer);
}

// ------------------------------------------------------------------------- //
// Returns 1 if the path is an absolute path; 0 otherwise.                   //
// ------------------------------------------------------------------------- //
int
Path::isAbsolute() {
  return absolute;
}

// ------------------------------------------------------------------------- //
// Creates a deep copy of the path object.                                   //
// It is the caller's responsibility to free the return value.               //
// ------------------------------------------------------------------------- //
Path*
Path::clone() {
  Path *np = new Path();
  np -> absolute = absolute;
  SegmentList::iterator it = segments -> begin();
  for (; it != segments -> end(); ++it) {
    np -> segments -> push_back(*it);
  }
  return np;
}
