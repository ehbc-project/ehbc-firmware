#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <stdint.h>

__attribute__((packed))
struct stack_frame {
    uint16_t sr;
    uint32_t pc;
    uint16_t format : 4;
    uint16_t vector : 12;
    union {
        struct {
            uint32_t instr_addr;
        } fmt2h;
        struct {
            uint32_t instr_addr;
            uint16_t internal[4];
        } fmt9h;
        struct {
            uint16_t internal0;
            uint16_t status;
            uint16_t pipeline_c;
            uint16_t pipeline_b;
            uint32_t fault_addr;
            uint16_t internal1[2];
            uint32_t output_buf;
            uint16_t internal2[2];
        } fmtAh;
        struct {
            uint16_t internal0;
            uint16_t status;
            uint16_t pipeline_c;
            uint16_t pipeline_b;
            uint32_t fault_addr;
            uint16_t internal1[2];
            uint32_t output_buf;
            uint16_t internal2[4];
            uint32_t stage_b_addr;
            uint16_t internal3[2];
            uint32_t input_buf;
            uint16_t internal4[3];
            uint16_t version : 4;
            uint16_t internal5 : 12;
            uint16_t internal6[18];
        } fmtBh;
    };
};

#endif // __EXCEPTION_H__
