#ifndef __FUNC_TYPE_H__
#define __FUNC_TYPE_H__

/*

Macros for higher-order function in c

Usages:

     Structure for storing capture values
             v
struct foo_capture {
    int cap1, cap2, cap3;
};

                    The struct type defined above
          Return value type |                      Arguments
        Function name  |    |                   ,------|-----,
                  v    v    v                   v      v     v
DEFINE_FUNC_TYPE(foo, int, struct foo_capture, arg1, arg2, arg3)
{
            Use CAPTURED() macro to access the captured value
             v
    return CAPTURED(cap1) + CAPTURED(cap2) + CAPTURED(cap3) + arg1 + arg2 + arg3;
}

   Use FUNC_TYPE() macro to get a function as argument
              v
int func(FUNC_TYPE(foo) func, int arg1, int arg2, int arg3)
{
             Function variable name
        Call function  |      Arguments
              |        |    ,-----|-----,
              v        |    v     v     v
    return FUNC_CALL(func, arg1, arg2, arg3);
}

int main(void)
{
                Capture and pass value
                        v
    int value = func(FUNC_PASS(foo, 1, 2, 3), 4, 5, 6);
    printf("%d = 21\n", value);
}

*/

#define FUNC_TYPE(type) __func_type_##type
#define FUNC_CAPTURE_TYPE(type) __func_capture_##type

#define DEFINE_FUNC_TYPE(type, return_type, capture_type, ...)                 \
    typedef capture_type FUNC_CAPTURE_TYPE(type);                              \
    typedef struct {                                                           \
        return_type (*func)(capture_type *capture, __VA_ARGS__);               \
        capture_type capture;                                                  \
    } FUNC_TYPE(type);                                                         \
    return_type type(capture_type *capture, __VA_ARGS__)

#define CAPTURED(name) (capture->name)

#define FUNC_PASS(type, ...) (FUNC_TYPE(type)){ type, (FUNC_CAPTURE_TYPE(type)){ __VA_ARGS__ } }

#define FUNC_CALL(func_obj, ...) (func_obj.func(&func_obj.capture, __VA_ARGS__))

#endif // __FUNC_TYPE_H__
