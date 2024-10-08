#ifndef __M68K_MOVEC_H__
#define __M68K_MOVEC_H__

#define GET_DFC(x) __asm__ volatile ("movec %/dfc,%0" : "=d"(x))
#define SET_DFC(x) __asm__ volatile ("movec %0,%/dfc" : : "d"(x))

#define GET_SFC(x) __asm__ volatile ("movec %/sfc,%0" : "=d"(x))
#define SET_SFC(x) __asm__ volatile ("movec %0,%/sfc" : : "d"(x))

#define GET_USP(x) __asm__ volatile ("movec %/usp,%0" : "=g"(x))
#define SET_USP(x) __asm__ volatile ("movec %0,%/usp" : : "r"(x))

#define GET_VBR(x) __asm__ volatile ("movec %/vbr,%0" : "=g"(x))
#define SET_VBR(x) __asm__ volatile ("movec %0,%/vbr" : : "r"(x))

#if defined(__mc68020__) || defined(__mc68030__) || defined(__mc68040__)

#define GET_CACR(x) __asm__ volatile ("movec %/cacr,%0" : "=d"(x))
#define SET_CACR(x) __asm__ volatile ("movec %0,%/cacr" : : "d"(x))

#define GET_CAAR(x) __asm__ volatile ("movec %/caar,%0" : "=d"(x))
#define SET_CAAR(x) __asm__ volatile ("movec %0,%/caar" : : "d"(x))

#define GET_MSP(x) __asm__ volatile ("movec %/msp,%0" : "=d"(x))
#define SET_MSP(x) __asm__ volatile ("movec %0,%/msp" : : "d"(x))

#define GET_ISP(x) __asm__ volatile ("movec %/isp,%0" : "=d"(x))
#define SET_ISP(x) __asm__ volatile ("movec %0,%/isp" : : "d"(x))

#endif // defined(__mc68020__) || defined(__mc68030__) || defined(__mc68040__)

#endif // __M68K_MOVEC_H__
