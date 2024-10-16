cmake_minimum_required(VERSION 3.13)

target_sources(libehbcfw PRIVATE assert.c bswap.c ctype.c elf.c errno.c mktime.c mpool.c disk.c strftime.c printf.c snprintf.c sprintf.c vprintf.c vsprintf.c vsnprintf.c vsprintf.c strerror.c strncasecmp.c strncmp.c strncpy.c strnlen.c memcpy.c memset.c memset16.c syscall_aio.c syscall_video.c syscall_storage.c syscall_rtc.c)
