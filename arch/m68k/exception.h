#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <stdint.h>

__attribute__((packed))
struct stack_frame {
    uint16_t sr;
    void *pc;
    uint16_t format : 4;
    uint16_t vector : 12;
    union {
        struct {
            void *instr_addr;
        } fmt2h;
        struct {
            void *instr_addr;
            uint16_t internal[4];
        } fmt9h;
        struct {
            uint16_t internal0;
            uint16_t status;
            uint16_t pipeline_c;
            uint16_t pipeline_b;
            void *fault_addr;
            uint16_t internal1[2];
            uint32_t output_buf;
            uint16_t internal2[2];
        } fmtAh;
        struct {
            uint16_t internal0;
            uint16_t status;
            uint16_t pipeline_c;
            uint16_t pipeline_b;
            void *fault_addr;
            uint16_t internal1[2];
            uint32_t output_buf;
            uint16_t internal2[4];
            void *stage_b_addr;
            uint16_t internal3[2];
            uint32_t input_buf;
            uint16_t internal4[3];
            uint16_t version : 4;
            uint16_t internal5 : 12;
            uint16_t internal6[18];
        } fmtBh;
    };
};

__attribute__((packed))
struct exception_vector {
    void *initial_sp;
    void *initial_pc;
    void *bus_error;
    void *addr_error;

    void *illegal_instr;
    void *divzero;
    void *chk_instr;
    void *misc_trap_instr;

    void *privilege_violation;
    void *trace;
    void *line_1010;
    void *line_1111;

    void *reserved0;
    void *coproc_protocol_violation;
    void *format_error;
    void *uninitialized_interrupt;

    void *reserved1;
    void *reserved2;
    void *reserved3;
    void *reserved4;
    void *reserved5;
    void *reserved6;
    void *reserved7;
    void *reserved8;
    
    void *spurious_interrupt;
    void *autovector1;
    void *autovector2;
    void *autovector3;
    void *autovector4;
    void *autovector5;
    void *autovector6;
    void *autovector7;

    void *trap0;
    void *trap1;
    void *trap2;
    void *trap3;
    void *trap4;
    void *trap5;
    void *trap6;
    void *trap7;
    void *trap8;
    void *trap9;
    void *trap10;
    void *trap11;
    void *trap12;
    void *trap13;
    void *trap14;
    void *trap15;

    void *fpcp_bra_set_unordered;
    void *fpcp_inexact;
    void *fpcp_divzero;
    void *fpcp_underflow;
    void *fpcp_operand_error;
    void *fpcp_overflow;
    void *fpcp_signaling_nan;
    void *fpcp_unimpl_data_type;

    void *mmu_config_error;
    void *mmu_illegal_op_error;
    void *mmu_access_violation;
    void *reserved9;

    void *reserved10;
    void *reserved11;
    void *reserved12;
    void *reserved13;

    void *userdef[192];
};

#endif // __EXCEPTION_H__
