// ------------------------------------------------------------------------- //
//                                                                           //
// CS252 Lab03 - Shell                                                       //
// Copyright © 2015 Denis Luchkin-Zhou                                       //
//                                                                           //
// path.h                                                                    //
// This file contains definitions for the class handling file path           //
// manipulation. This class forms the cornerstone of globbing.               //
//                                                                           //
// ------------------------------------------------------------------------- //

#ifndef PATH_H_
#define PATH_H_

#include <list>

// ------------------------------------------------------------------------- //
// Shorthand for the internal list type that holds path segments.            //
// ------------------------------------------------------------------------- //
typedef std::list<char*> SegmentList;

// ------------------------------------------------------------------------- //
// Path class, the one that bends all paths to its will.                     //
// ------------------------------------------------------------------------- //
class Path {
private:
  int           absolute;      // Flag indicating whether the path is absolute
  SegmentList  *segments;      // Segments of the path

  void  init(const char*);     // Initializes the class instance

public:

  Path();                      // Default constructor. Creates an empty Path.
  Path(const char*);           // Constructor. Creates a path from a path
                               // string. Creates its own copy of path string.
  ~Path();                     // Destructor. Cleans up the mess.

  void  push(const char*);     // Pushes a path string into the path.
  void  push(Path*);           // Pushes another path object on the path.
  void  pushd(const char*);    // Pushes a single segment into the path.
  char* popd();                // Pops the first segment of the path.

  void  reduce();              // Resolves '.' and '..' segments where possible.

  char* str();                 // Returns a string representation of the path.
  int   isAbsolute();          // Returns the Path::absolute flag.

  Path* clone();               // Creates a deep copy of the path object.

  // ----------------------------------------------------------------------- //
  // STATIC METHODS: mostly utility methods used by Path.                    //
  // ----------------------------------------------------------------------- //
  static
  void  unescape(char*);       // Removes backslashes from escape sequences.
                               // Does not interpret excape sequences such as
                               // \n or \t
  static
  void  replace(std::string*,  // Replaces all occurences of given substring
                const char* ,  // in a std::string.
                const char* );
  static
  void  trimend(char*, char);  // Removes all occurences of the specified
                               // character from the end of a string.
  static
  char* trim(const char*,      // Removes all occurences of the specified
             char);            // character from both ends of a string.
};

#endif
