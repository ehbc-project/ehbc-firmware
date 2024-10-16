#ifndef __ASSERT_H__
#define __ASSERT_H__

# ifdef NDEBUG
#   define assert(expr) ((void)0)

# else

#define static_assert _Static_assert

__attribute__((noreturn))
extern void __assert_fail(const char *assertion, const char *file,
    unsigned int line, const char *function);

#   define assert(expr) ((expr) ? (void)0 : \
        __assert_fail(#expr, __FILE__, __LINE__, __func__))

#endif

#endif // __ASSERT_H__
