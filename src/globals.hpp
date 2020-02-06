/* Copyright 2017 Columbia University, SLD Group */

//
// globals.h - Robert Margelli
// This file several defines and constants: number of registers, data width, opcodes etc.
//

#ifndef GLOBALS_H
#define GLOBALS_H

#include "systemc.h"

// Miscellanous sizes. Most of these can be changed to obtain new architectures.
#define XLEN        32      // Register width. 32 or 64. Currently only 32 is supported.
#define REG_NUM     32      // Number of registers in regfile (x0-x31)    // CONST
#define REG_ADDR    5       // Number of reg file address lines   // CONST
#define IMEM_SIZE   2048    // Size of instruction memory
#define DMEM_SIZE   2048    // Size of data memory
#define DATA_SIZE   32      // Size of data in DMEM   // CONST
#define PC_LEN      32      // Width of PC register
#define ALUOP_SIZE  5       // Size of aluop signal.
#define ALUSRC_SIZE 2       // Size of alusrc signal.
#define BYTE        8       // 8-bits.
#define ZIMM_SIZE   5       // Bit-length of zimm field in CSRRWI, CSRRSI, CSRRCI
#define SHAMT       5       // Number of bits used for the shift value in shift operations.

// Values for CSR and traps
#define LOG2_NUM_CAUSES 3   // Log2 of number of trap causes
#define CSR_NUM         11  // Number of CSR registers (including Performance Counters).
#define CSR_IDX_LEN     4   // Log2 of CSR_NUM      // TODO: this should be rewritten into something like log2(CSR_NUM)
#define PRF_CNT_NUM     1   // Number of Performance Counters.
#define CSR_ADDR        12  // CSRs are on a 12-bit addressing space.
#define LOG2_CSR_OP_NUM 2   // Log2 of number of operations on CSR.
#define CSR_OP_WR       1   // CSR write operation.
#define CSR_OP_SET      2   // CSR set operation.
#define CSR_OP_CLR      3   // CSR clear operation.
#define CSR_OP_NULL     0   // Not a CSR operation.

// Instruction fields sizes. All contant.
#define INSN_LEN    32
#define OPCODE_SIZE 5       // Note: in reality opcodes are on 7 bits but bits [1:0] are statically at '1'. This gives us a saving of approximately 300 in 'Total Area' of the fedec stage.
#define FUNCT7_SIZE 7
#define FUNCT3_SIZE 3
#define RS1_SIZE    5
#define RS2_SIZE    5
#define RD_SIZE     5
#define IMM_ITYPE   12	// imm[11:0]
#define IMM_STYPE1  7	// imm[11:5]
#define IMM_STYPE2  5	// imm[4:0]
#define IMM_SBTYPE1 7	// imm[12|10:5]
#define IMM_SBTYPE2 5	// imm[4:1|11]
#define IMM_UTYPE   20	// imm[31:12]
#define IMM_UJTYPE  20	// imm[20|10:1|11|19:12]

/* Supported instructions 45+8=53 :
*   add, sll, slt, sltu, xor, srl, or, and, sub, sra,
*   addi, slti, sltiu, xori, ori, andi, slli, srli, srai,
*   sb, sh, sw, lb, lh, lw, lbu, lhu,
*   beq, bne, blt, bge, bltu, bgeu,
*   lui, auipc, jalr, jal,
*   ebreak, ecall, csrrw, csrrs, csrrc, csrrwi, csrrsi, csrrci,
*   mul, mulh, mulhsu, mulhu, div, divu, rem, remu
*
*   i.e. all RV32I except {FENCE, FENCE.I} and all RV32M
*   NB. ETH/Bologna's RI5CY does not support FENCE and FENCE.I
*/

/* Opcodes as integers. For control word generation switch case. */
#define OPC_ADD     12       // Original value is 51, but we trim the opcode's LSBs which are statically at 2'b11 for all instructions.
#define OPC_SLL     OPC_ADD
#define OPC_SLT     OPC_ADD
#define OPC_SLTU    OPC_ADD
#define OPC_XOR     OPC_ADD
#define OPC_SRL     OPC_ADD
#define OPC_OR      OPC_ADD
#define OPC_AND     OPC_ADD
#define OPC_SUB     OPC_ADD
#define OPC_SRA     OPC_ADD
#define OPC_MUL     OPC_ADD
#define OPC_MULH    OPC_ADD
#define OPC_MULHSU  OPC_ADD
#define OPC_MULHU   OPC_ADD
#define OPC_DIV     OPC_ADD
#define OPC_DIVU    OPC_ADD
#define OPC_REM     OPC_ADD
#define OPC_REMU    OPC_ADD

#define OPC_ADDI    4          // Original value is 19, but we trim the opcode's LSBs which are statically at 2'b11 for all instructions.
#define OPC_SLTI    OPC_ADDI
#define OPC_SLTIU   OPC_ADDI
#define OPC_XORI    OPC_ADDI
#define OPC_ORI     OPC_ADDI
#define OPC_ANDI    OPC_ADDI
#define OPC_SLLI    OPC_ADDI
#define OPC_SRLI    OPC_ADDI
#define OPC_SRAI    OPC_ADDI

#define OPC_SB      8          // Original value is 35, but we trim the opcode's LSBs which are statically at 2'b11 for all instructions.
#define OPC_SH      OPC_SB
#define OPC_SW      OPC_SB

#define OPC_LB      0          // Original value is 3, but we trim the opcode's LSBs which are statically at 2'b11 for all instructions.
#define OPC_LH      OPC_LB
#define OPC_LW      OPC_LB
#define OPC_LBU     OPC_LB
#define OPC_LHU     OPC_LB

#define OPC_BEQ     24         // Original value is 99, but we trim the opcode's LSBs which are statically at 2'b11 for all instructions.
#define OPC_BNE     OPC_BEQ
#define OPC_BLT     OPC_BEQ
#define OPC_BGE     OPC_BEQ
#define OPC_BLTU    OPC_BEQ
#define OPC_BGEU    OPC_BEQ

#define OPC_LUI     13         // Original value is 55, but we trim the opcode's LSBs which are statically at 2'b11 for all instructions.

#define OPC_AUIPC   5          // Original value is 23, but we trim the opcode's LSBs which are statically at 2'b11 for all instructions.

#define OPC_JAL     27         // Original value is 111, but we trim the opcode's LSBs which are statically at 2'b11 for all instructions.

#define OPC_JALR    25         // Original value is 103, but we trim the opcode's LSBs which are statically at 2'b11 for all instructions.

#define OPC_SYSTEM  28         // Original value is 115, but we trim the opcode's LSBs which are statically at 2'b11 for all instructions.
#define OPC_EBREAK  OPC_SYSTEM
#define OPC_ECALL   OPC_SYSTEM
#define OPC_CSRRW   OPC_SYSTEM
#define OPC_CSRRS   OPC_SYSTEM
#define OPC_CSRRC   OPC_SYSTEM
#define OPC_CSRRWI   OPC_SYSTEM
#define OPC_CSRRSI   OPC_SYSTEM
#define OPC_CSRRCI   OPC_SYSTEM

/* Funct3 as integers. For control word generation switch case. */
#define FUNCT3_ADD  0
#define FUNCT3_SLL  1
#define FUNCT3_SLT  2
#define FUNCT3_SLTU 3
#define FUNCT3_XOR  4
#define FUNCT3_SRL  5
#define FUNCT3_OR   6
#define FUNCT3_AND  7

#define FUNCT3_SUB  0
#define FUNCT3_SRA  5

#define FUNCT3_MUL      0
#define FUNCT3_MULH     1
#define FUNCT3_MULHSU   2
#define FUNCT3_MULHU    3
#define FUNCT3_DIV      4
#define FUNCT3_DIVU     5
#define FUNCT3_REM      6
#define FUNCT3_REMU     7

#define FUNCT3_ADDI     0
#define FUNCT3_SLTI     2
#define FUNCT3_SLTIU    3
#define FUNCT3_XORI     4
#define FUNCT3_ORI      6
#define FUNCT3_ANDI     7
#define FUNCT3_SLLI     1
#define FUNCT3_SRLI     5
#define FUNCT3_SRAI     5

#define FUNCT3_SB   0
#define FUNCT3_SH   1
#define FUNCT3_SW   2

#define FUNCT3_LB   0
#define FUNCT3_LH   1
#define FUNCT3_LW   2
#define FUNCT3_LBU  4
#define FUNCT3_LHU  5

#define FUNCT3_BEQ   0
#define FUNCT3_BNE   1
#define FUNCT3_BLT   4
#define FUNCT3_BGE   5
#define FUNCT3_BLTU  6
#define FUNCT3_BGEU  7

#define FUNCT3_JALR  0

#define FUNCT3_EBREAK	0
#define FUNCT3_ECALL 	0
#define FUNCT3_CSRRW  	1
#define FUNCT3_CSRRS  	2
#define FUNCT3_CSRRC  	3
#define FUNCT3_CSRRWI  	5
#define FUNCT3_CSRRSI  	6
#define FUNCT3_CSRRCI  	7

/* Funct7 as integers. For control word generation switch case. */
#define FUNCT7_ADD      0
#define FUNCT7_SLL      FUNCT7_ADD
#define FUNCT7_SLT      FUNCT7_ADD
#define FUNCT7_SLTU     FUNCT7_ADD
#define FUNCT7_XOR      FUNCT7_ADD
#define FUNCT7_SRL      FUNCT7_ADD
#define FUNCT7_OR       FUNCT7_ADD
#define FUNCT7_AND      FUNCT7_ADD

#define FUNCT7_SUB      32
#define FUNCT7_SRA      FUNCT7_SUB

#define FUNCT7_MUL      1
#define FUNCT7_MULH     FUNCT7_MUL
#define FUNCT7_MULHSU   FUNCT7_MUL
#define FUNCT7_MULHU    FUNCT7_MUL
#define FUNCT7_DIV      FUNCT7_MUL
#define FUNCT7_DIVU     FUNCT7_MUL
#define FUNCT7_REM      FUNCT7_MUL
#define FUNCT7_REMU     FUNCT7_MUL

#define FUNCT7_SLLI     0
#define FUNCT7_SRLI     FUNCT7_SLLI
#define FUNCT7_SRAI     32

#define FUNCT7_EBREAK	0	// Note: strictly speaking ebreak and ecall don't have a funct7 field, but their [31-20] bits
#define FUNCT7_ECALL	1	// are used to distinguish between them. I call these FUNCT7 for the sake of modularity.

/* ALUOPS */
#define ALUOP_NULL      0

#define ALUOP_ADD       1
#define ALUOP_SLL       2
#define ALUOP_SLT       3
#define ALUOP_SLTU      4
#define ALUOP_XOR       5
#define ALUOP_SRL       6
#define ALUOP_OR        7
#define ALUOP_AND       8

#define ALUOP_SUB       9
#define ALUOP_SRA       10

#define ALUOP_MUL       11
#define ALUOP_MULH      12
#define ALUOP_MULHSU    13
#define ALUOP_MULHU     14
#define ALUOP_DIV       15
#define ALUOP_DIVU      16
#define ALUOP_REM       17
#define ALUOP_REMU      18

// Integer immediate operation's aluops coincide with their r-type counterparts
#define ALUOP_ADDI ALUOP_ADD
#define ALUOP_SLTI ALUOP_SLT
#define ALUOP_SLTIU ALUOP_SLTU
#define ALUOP_XORI ALUOP_XOR
#define ALUOP_ORI ALUOP_OR
#define ALUOP_ANDI ALUOP_AND
#define ALUOP_SLLI 19
#define ALUOP_SRLI 20
#define ALUOP_SRAI 21

#define ALUOP_LUI   22
#define ALUOP_AUIPC 23
#define ALUOP_JAL   24
#define ALUOP_JALR  ALUOP_JAL   // like JAL, the ALU operation is < rd = pc + 4 >

#define ALUOP_CSRRW   25
#define ALUOP_CSRRS   26
#define ALUOP_CSRRC   27
#define ALUOP_CSRRWI  28
#define ALUOP_CSRRSI  29
#define ALUOP_CSRRCI  30

/* ALU Source discrimination values */
#define ALUSRC_RS2      0
#define ALUSRC_IMM_I    1
#define ALUSRC_IMM_S    2
#define ALUSRC_IMM_U    3

/* Load and store discrimination values to be assigned to the ld or st signals */
#define NO_LOAD  5
#define LB_LOAD  0
#define LH_LOAD  1
#define LW_LOAD  2
#define LBU_LOAD 3
#define LHU_LOAD 4

#define NO_STORE 3
#define SB_STORE 0
#define SH_STORE 1
#define SW_STORE 2

/* Trap causes: see page 35 of RISC-V privileged ISA draft V1.10. */
#define NULL_CAUSE      10  // 10 is actually reserved in the specs but we use it to indicate no cause.
#define EBREAK_CAUSE    3
#define ECALL_CAUSE     11
#define ILL_INSN_CAUSE  2

/* Control Status Registers' addresses */
#define USTATUS_A     0x000
#define MSTATUS_A     0x300
#define MISA_A        0x301
#define MTVECT_A      0x305
#define MEPC_A        0x341
#define MCAUSE_A      0x342
#define MCYCLE_A      0xB00
#define MARCHID_A     0xF12
#define MIMPID_A      0xF13
#define MINSTRET_A    0xF02
#define MHARTID_A     0xF14

#define USTATUS_I     0
#define MSTATUS_I     1
#define MISA_I        2
#define MTVECT_I      3
#define MEPC_I        4
#define MCAUSE_I      5
#define MCYCLE_I      6
#define MARCHID_I     7
#define MIMPID_I      8
#define MINSTRET_I    9
#define MHARTID_I     10

#endif
