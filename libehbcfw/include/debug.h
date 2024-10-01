#ifndef __DEBUG_H__
#define __DEBUG_H__

void debug_write(const char *str);

__attribute__((format(printf, 1, 2)))
void debug_printf(const char *fmt, ...);

#endif // __DEBUG_H__
