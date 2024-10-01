#ifndef MACROS_H__
#define MACROS_H__

#include <stdint.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define TEST_FLAG(val, mask) (((val) & mask) == mask)

#endif  // MACROS_H__
