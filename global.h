#ifndef GLOBAL_H_
#define GLOBAL_H_

// Name of this shell
#define SH_NAME "myshell"

// Disables features so that different parts of the lab can be
// independently tested
#define FLVL_PART1 0
#define FLVL_PART2 1
#define FLVL_PART3 2
#define FLVL_EXTRA 3

#ifndef FEATURE_LEVEL
#define FEATURE_LEVEL 4
#endif

#endif
