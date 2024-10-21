cmake_minimum_required(VERSION 3.13)

# assert.h
target_sources(libehbcfw PRIVATE assert.c)

# ctype.h
target_sources(libehbcfw PRIVATE ctype.c)

# errno.h
target_sources(libehbcfw PRIVATE errno.c)

# locale.h
target_sources(libehbcfw PRIVATE setlocale.c)

# stdio.h
target_sources(libehbcfw PRIVATE stdio.c fgets.c fprintf.c printf.c putchar.c snprintf.c sprintf.c vprintf.c vsprintf.c vsnprintf.c vsprintf.c)

# stdlib.h
target_sources(libehbcfw PRIVATE atexit.c abs.c atoi.c qsort.c exit.c malloc.c realloc.c free.c rand.c strtod.c strtol.c strtoll.c)

# string.h
target_sources(libehbcfw PRIVATE strchr.c strrchr.c strerror.c strncasecmp.c strncat.c strncmp.c strncpy.c strnlen.c strtok.c memcmp.c memcpy.c memset.c memset16.c)

# time.h
target_sources(libehbcfw PRIVATE time.c mktime.c strftime.c localtime.c asctime.c)
