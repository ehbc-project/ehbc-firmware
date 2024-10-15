#ifndef __M68K_LIBEHBCFW_SYSCALL_H__
#define __M68K_LIBEHBCFW_SYSCALL_H__

static inline long __syscall0(unsigned int cat, unsigned int num)
{
    register unsigned long d0 __asm__("d0") = ((cat & 0xFFFF) << 16) | (num & 0xFFFF);
    __asm__ volatile ("trap #0" : "+r"(d0) : : "memory");
    return d0;
}

static inline long __syscall1(unsigned int cat, unsigned int num, long a)
{
    register unsigned long d0 __asm__("d0") = ((cat & 0xFFFF) << 16) | (num & 0xFFFF);
    register unsigned long d1 __asm__("d1") = a;
    __asm__ volatile ("trap #0" : "+r"(d0) : "r"(d1) : "memory");
    return d0;
}

static inline long __syscall2(unsigned int cat, unsigned int num, long a, long b)
{
    register unsigned long d0 __asm__("d0") = ((cat & 0xFFFF) << 16) | (num & 0xFFFF);
    register unsigned long d1 __asm__("d1") = a;
    register unsigned long d2 __asm__("d2") = b;
    __asm__ volatile ("trap #0" : "+r"(d0) : "r"(d1), "r"(d2) : "memory");
    return d0;
}

static inline long __syscall3(unsigned int cat, unsigned int num, long a, long b, long c)
{
    register unsigned long d0 __asm__("d0") = ((cat & 0xFFFF) << 16) | (num & 0xFFFF);
    register unsigned long d1 __asm__("d1") = a;
    register unsigned long d2 __asm__("d2") = b;
    register unsigned long d3 __asm__("d3") = c;
    __asm__ volatile ("trap #0" : "+r"(d0) : "r"(d1), "r"(d2), "r"(d3) : "memory");
    return d0;
}

static inline long __syscall4(unsigned int cat, unsigned int num, long a, long b, long c, long d)
{
    register unsigned long d0 __asm__("d0") = ((cat & 0xFFFF) << 16) | (num & 0xFFFF);
    register unsigned long d1 __asm__("d1") = a;
    register unsigned long d2 __asm__("d2") = b;
    register unsigned long d3 __asm__("d3") = c;
    register unsigned long d4 __asm__("d4") = d;
    __asm__ volatile ("trap #0" : "+r"(d0) : "r"(d1), "r"(d2), "r"(d3), "r"(d4) : "memory");
    return d0;
}

#endif // __M68K_LIBEHBCFW_SYSCALL_H__
