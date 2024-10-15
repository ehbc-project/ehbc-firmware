#ifndef __SETJMP_H__
#define __SETJMP_H__

#include <asm/setjmp.h>

int setjmp(jmp_buf env);
void longjmp(jmp_buf env, int val);

#endif // __SETJMP_H__
