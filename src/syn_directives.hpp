/* Copyright 2017 Columbia University, SLD Group */

//
// syn_directives.h - Robert Margelli
// Header file with synthesis directives for DSE.
//

#ifndef SYN_DIRECTIVES_H
#define SYN_DIRECTIVES_H


#define FLAT_PROGRAM HLS_FLATTEN_ARRAY(program)
#define FLAT_REGFILE HLS_FLATTEN_ARRAY(regfile)
#define FLAT_SENTINEL HLS_FLATTEN_ARRAY(sentinel)
#define MAP_ICACHE HLS_MAP_TO_MEMORY(imem, "hl5_block_1w1r")
#define MAP_DCACHE HLS_MAP_TO_MEMORY(dmem, "hl5_block_1w1r")
#define PROTO_FEDEC_RST HLS_DEFINE_PROTOCOL("fedec_reset")
#define PROTO_FEDEC_BODY HLS_DEFINE_PROTOCOL("fedec_body")
#define PROTO_EXE_RST HLS_DEFINE_PROTOCOL("execute_reset")
#define PROTO_PERF_RST HLS_DEFINE_PROTOCOL("perfcute_reset")
#define PROTO_EXE_BODY HLS_DEFINE_PROTOCOL("execute_body")
#define PROTO_PERF_BODY HLS_DEFINE_PROTOCOL("perfcute_body")
#define PROTO_MEMWB_RST HLS_DEFINE_PROTOCOL("memwb_reset")
#define PROTO_MEMWB_BODY HLS_DEFINE_PROTOCOL("memwb_body")
#define BREAK_DMEM_DEP HLS_BREAK_ARRAY_DEPENDENCY(dmem)

// Division unroll
#if defined(DIV_UROLL4)
    #define DIV_UNROLL HLS_UNROLL_LOOP(AGGRESSIVE, 4, "div_loop_unroll_4")
#elif defined(DIV_UROLL2)
    #define DIV_UNROLL HLS_UNROLL_LOOP(AGGRESSIVE, 2, "div_loop_unroll_2")
#else
    #define DIV_UNROLL
#endif

// Multiplication split
#if defined(MUL_SPLIT8)
    #define MUL_SPLIT HLS_SPLIT_MULTIPLY(8, "split_mul_8")
#elif defined(MUL_SPLIT4)
    #define MUL_SPLIT HLS_SPLIT_MULTIPLY(4, "split_mul_4")
#elif defined(MUL_SPLIT2)
    #define MUL_SPLIT HLS_SPLIT_MULTIPLY(2, "split_mul_2")
#else
    #define MUL_SPLIT
#endif

// Addition split
#if defined(ADD_SPLIT8)
    #define ADD_SPLIT HLS_SPLIT_ADD(8, "split_add_8")
#elif defined(ADD_SPLIT4)
    #define ADD_SPLIT HLS_SPLIT_ADD(4, "split_add_4")
#elif defined(ADD_SPLIT2)
    #define ADD_SPLIT HLS_SPLIT_ADD(2, "split_add_2")
#else
    #define ADD_SPLIT
#endif

#endif // SYN_DIRECTIVES_H
