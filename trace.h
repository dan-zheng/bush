#ifndef TRACE_H
#define TRACE_H

#include <stdio.h>

#define COLOR_RED     "\x1b[31m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_RESET   "\x1b[0m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_MAGENTA "\x1b[35m"

#define RED(s)     COLOR_RED     s COLOR_RESET
#define BLUE(s)    COLOR_BLUE    s COLOR_RESET
#define CYAN(s)    COLOR_CYAN    s COLOR_RESET
#define GREEN(s)   COLOR_GREEN   s COLOR_RESET
#define YELLOW(s)  COLOR_YELLOW  s COLOR_RESET
#define MAGENTA(s) COLOR_MAGENTA s COLOR_RESET

#ifdef DEBUG

  #define DBG_TRACE(...) printf(COLOR_GREEN __VA_ARGS__); printf(COLOR_RESET);
  #define DBG_WARN(...) fprintf(stderr, COLOR_YELLOW __VA_ARGS__ COLOR_RESET);
  #define DBG_ERR(...) fprintf(stderr, COLOR_RED __VA_ARGS__ COLOR_RED);

#else

  #define DBG_TRACE(...)
  #define DBG_WARN(...)
  #define DBG_ERR(...)

#endif

  #define WARN(...) fprintf(stderr, COLOR_YELLOW __VA_ARGS__ COLOR_RESET);
  #define PANIC(...) fprintf(stderr, __VA_ARGS__); exit(1);

#endif
