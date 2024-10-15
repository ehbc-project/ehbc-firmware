#ifndef __ASM_SETJMP_H__
#define __ASM_SETJMP_H__

typedef struct __jmp_buf {
    long int d[7];  // data registers except D0
    void *a[6];  // address registers A0-A5
    void *fp;  // frame pointer register A6
    void *sp;  // stack pointer register A7

#if defined(__HAVE_68881__) || defined(__HAVE_FPU__)
    unsigned char f[96 / 8][8];  // IEEE 754 96-bit floating-point registers
#endif
} jmp_buf[1];

#endif // __ASM_SETJMP_H__
