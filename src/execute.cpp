/* Copyright 2017 Columbia University, SLD Group */

//
// execute.cpp - Robert Margelli
// Implementation of the execute stage
//

#include "execute.hpp"

u_div_res_t execute::udiv_func(sc_uint<XLEN> num, sc_uint<XLEN> den)
{
	sc_uint<XLEN> rem;
	sc_uint<XLEN> quotient;
	u_div_res_t u_div_res;

	rem = 0;
	quotient = 0;

DIVIDE_LOOP:
	for(sc_int<6> i = 31; i >= 0; i--){
		// Break EXE stage protocol for DSE
		HLS_BREAK_PROTOCOL("div_break_proto");
		DIV_UNROLL;

		const sc_uint<XLEN> mask = BIT(i);
		const sc_uint<XLEN> lsb = (mask & num) >> i;

		rem = rem << 1;
		rem = rem | lsb;

		if(rem >= den){
			rem -= den;
			quotient = quotient | mask;
		}
	}

	u_div_res.quotient = quotient;
	u_div_res.remainder = rem;

	return u_div_res;
}


div_res_t execute::div_func(sc_int<XLEN> num, sc_int<XLEN> den)
{
	bool num_neg;
	bool den_neg;
	div_res_t div_res;
	u_div_res_t u_div_res;

	num_neg = num < 0;
	den_neg = den < 0;

	if(num_neg)
		num = -num;
	if(den_neg)
		den = -den;

	u_div_res = udiv_func((sc_uint<XLEN>) num, (sc_uint<XLEN>) den);
	div_res.quotient = (sc_int<XLEN>)u_div_res.quotient;
	div_res.remainder = (sc_int<XLEN>)u_div_res.remainder;

	if(num_neg ^ den_neg)
		div_res.quotient = -div_res.quotient;
	else
		div_res.quotient =  div_res.quotient;

	return div_res;
}

void execute::perf_th(void)
{
PERF_RST:
	{
		PROTO_PERF_RST;

                csr[MCYCLE_I] = 0x0; // Cycle count (32-bits only for now)
		wait();
		wait();
	}

        while (true) {
            PROTO_PERF_BODY;
            csr[MCYCLE_I]++;
            wait();
        }

}

void execute::execute_th(void)
{
EXE_RST:
	{
		PROTO_EXE_RST;
		din.reset_get();
		dout.reset_put();
		output.tag = 0;

                csr[MISA_I] = 0x40001101; // RV32IMA
                csr[MARCHID_I] = 0x0; // Not implemented (should be assigned by RISC-V
                csr[MIMPID_I] = 0x0; // Not implemented (processor revision)
                csr[MHARTID_I] = 0x0; // Single thread (always 0)
                csr[MINSTRET_I] = 0x0; // Retired instructions
		wait();
		wait();
	}

EXE_BODY:
	while(true) {
		PROTO_EXE_BODY;
		MUL_SPLIT;
		ADD_SPLIT;

		// Get
		input = din.get();

		// Compute
		output.regwrite = input.regwrite;
		output.memtoreg = input.memtoreg;
		output.ld = input.ld;
		output.st = input.st;
		output.dest_reg = input.dest_reg;
		output.mem_datain = input.rs2;
		output.tag = input.tag;

                bool nop = false;
                if (input.regwrite == "0" &&
                    input.ld == NO_LOAD &&
                    input.st == NO_STORE &&
                    input.alu_op ==  (sc_bv<ALUOP_SIZE>)ALUOP_NULL)
                    nop = true;

#ifdef MUL64
		// 64-bit temporary multiplication result, for upper 32 bit multiplications (MULH, MULHU, MULHSU).
		__int64_t tmp_mul_res = 0;
#endif
#ifdef DIV
		// Temporary division results.
		div_res_t div_res = {0, 0};
		u_div_res_t u_div_res = {0, 0};
#endif
#ifdef CSR_LOGIC
		// Temporary CSR index
		sc_uint<CSR_IDX_LEN> csr_index = 0;
#endif

		// Sign extend the immediate operand for I-type instructions.
		sc_bv<XLEN> tmp_sigext_imm_i = sc_bv<XLEN>(0);
		if (input.imm_u.range(19, 19) == "1")
			// Extend with 1s
			tmp_sigext_imm_i = (sc_bv<20>("11111111111111111111"), (sc_bv<12>)input.imm_u.range(19, 8));
		else
			// Extend with 0s
			tmp_sigext_imm_i = (sc_bv<20>("00000000000000000000"), (sc_bv<12>)input.imm_u.range(19, 8));

		// Zero-fill the immediate operand for U-type instructions.
		sc_bv<XLEN> tmp_zerofill_imm_u = ((sc_bv<20>)input.imm_u.range(19, 0), sc_bv<12>("000000000000"));

		// ALU 2nd operand multiplexing based on ALUSRC signal.
		sc_bv<XLEN> tmp_rs2 = (sc_bv<XLEN>)0;

		if (input.alu_src == (sc_bv<ALUSRC_SIZE>)ALUSRC_RS2) {
			tmp_rs2 = input.rs2;
		} else if (input.alu_src == (sc_bv<ALUSRC_SIZE>)ALUSRC_IMM_I) {
			tmp_rs2 = tmp_sigext_imm_i;
		} else if (input.alu_src == (sc_bv<ALUSRC_SIZE>)ALUSRC_IMM_S) {
			// reconstructs imm_s from imm_u and rd
			sc_bv<12> imm_s = (sc_bv<7>(input.imm_u.range(19, 13)), input.dest_reg);
			tmp_rs2 = sign_extend_imm_s(imm_s);
		} else {
			// ALUSRC_IMM_U
			tmp_rs2 = tmp_zerofill_imm_u;
		}

		// ALU body
		switch(sc_uint<ALUOP_SIZE>(input.alu_op)) {
		case ALUOP_ADD: // ADD, ADDI, SB, SH, SW, LB, LH, LW, LBU, LHU.
			output.alu_res = sc_bv<XLEN>((sc_int<XLEN>)input.rs1 + (sc_int<XLEN>)tmp_rs2);
			break;
		case ALUOP_SLT: // SLT, SLTI
			if (sc_int<XLEN>(input.rs1) < sc_int<XLEN>(tmp_rs2))
				output.alu_res = (sc_bv<XLEN>)1;
			else
				output.alu_res = (sc_bv<XLEN>)0;
			break;
		case ALUOP_SLTU: // SLTU, SLTIU
			if (sc_uint<XLEN>(input.rs1) < sc_uint<XLEN>(tmp_rs2))
				output.alu_res = (sc_bv<XLEN>)1;
			else
				output.alu_res = (sc_bv<XLEN>)0;
			break;
		case ALUOP_XOR: // XOR, XORI
			output.alu_res = input.rs1 ^ tmp_rs2;
			break;
		case ALUOP_OR: // OR, ORI
			output.alu_res = input.rs1 | tmp_rs2;
			break;
		case ALUOP_AND: // AND, ANDI
			output.alu_res = input.rs1 & tmp_rs2;
			break;
		case ALUOP_SLL: // SLL
			output.alu_res = sc_uint<XLEN>(input.rs1) << sc_uint<SHAMT>((sc_bv<SHAMT>)tmp_rs2.range(4, 0));
			break;
		case ALUOP_SRL: // SRL
			output.alu_res = sc_uint<XLEN>(input.rs1) >> sc_uint<SHAMT>((sc_bv<SHAMT>)tmp_rs2.range(4, 0));
			break;
		case ALUOP_SRA: // SRA
			// >> is arith right sh. for sc_int operand
			output.alu_res = sc_int<XLEN>(input.rs1) >> sc_uint<SHAMT>((sc_bv<SHAMT>)tmp_rs2.range(4, 0));
			break;
		case ALUOP_SUB: // SUB
			output.alu_res = sc_bv<XLEN>((sc_int<XLEN>)input.rs1 - (sc_int<XLEN>)tmp_rs2);
			break;
		case ALUOP_SLLI: // SLLI
			output.alu_res = sc_uint<XLEN>(input.rs1) << sc_uint<SHAMT>((sc_bv<SHAMT>)tmp_rs2.range(24, 20));
			break;
		case ALUOP_SRLI: // SRLI
			output.alu_res = sc_uint<XLEN>(input.rs1) >> sc_uint<SHAMT>((sc_bv<SHAMT>)tmp_rs2.range(24, 20));
			break;
		case ALUOP_SRAI: // SRAI
			// >> is arith right sh. for sc_int operand
			output.alu_res = sc_int<XLEN>(input.rs1) >> sc_uint<SHAMT>((sc_bv<SHAMT>)tmp_rs2.range(24, 20));
			break;
		case ALUOP_LUI: // LUI
			// zerofill_imm_u
			output.alu_res = tmp_rs2;
			break;
		case ALUOP_AUIPC: // AUIPC
			// zerofill_imm_u + pc
			output.alu_res = sc_bv<XLEN>((sc_int<XLEN>)tmp_rs2 + (sc_int<XLEN>)input.pc);
			break;
		case ALUOP_JAL: // JAL, JALR
			// link register update
			output.alu_res = sc_bv<XLEN>((sc_int<XLEN>)input.pc + 4);
			break;
#ifdef MUL32
		case ALUOP_MUL: // MUL: signed * signed, return lower 32 bits
			output.alu_res = (sc_int<XLEN>)input.rs1 * (sc_int<XLEN>)tmp_rs2;;
			break;
#endif
#ifdef MUL64
		case ALUOP_MULH: // MULH: signed * signed, return upper 32 bits
			tmp_mul_res = input.rs1.to_int() * tmp_rs2.to_int();
			output.alu_res = sc_bv<XLEN*2>(sc_int<XLEN*2>(tmp_mul_res)).range((XLEN*2)-1, XLEN);
			break;
		case ALUOP_MULHSU: // MULHSU: signed * unsigned, return upper 32 bits
			tmp_mul_res = input.rs1.to_int() * tmp_rs2.to_uint();
			output.alu_res = sc_bv<XLEN*2>(sc_int<XLEN*2>(tmp_mul_res)).range((XLEN*2)-1, XLEN);
			break;
		case ALUOP_MULHU: // MULHU: unsigned * unsigned, return upper 32 bits
			tmp_mul_res = input.rs1.to_uint() * tmp_rs2.to_uint();
			output.alu_res = sc_bv<XLEN*2>(sc_int<XLEN*2>(tmp_mul_res)).range((XLEN*2)-1, XLEN);
			break;
#endif
#ifdef DIV
		case ALUOP_DIV: // DIV calls div_func
			div_res = div_func((sc_int<XLEN>)input.rs1, (sc_int<XLEN>)tmp_rs2);
			output.alu_res = (sc_bv<XLEN>)div_res.quotient;
			break;
		case ALUOP_DIVU: // DIVU calls udiv_func
			u_div_res = udiv_func((sc_uint<XLEN>)input.rs1, (sc_uint<XLEN>)tmp_rs2);
			output.alu_res = (sc_bv<XLEN>)u_div_res.quotient;
			break;
#endif
#ifdef REM
		case ALUOP_REM: // REM calls div_func
			div_res = div_func((sc_int<XLEN>)input.rs1, (sc_int<XLEN>)tmp_rs2);
			output.alu_res = (sc_bv<XLEN>)div_res.remainder;
			break;
		case ALUOP_REMU: // REMU calls udiv_func
			u_div_res = udiv_func((sc_uint<XLEN>)input.rs1, (sc_uint<XLEN>)tmp_rs2);
			output.alu_res = (sc_bv<XLEN>)u_div_res.remainder;
			break;
#endif
#ifdef CSR_LOGIC
			// All CSRx instructions exploit imm_u[19:8] to get the csr address.
			// This avoids having 12 more bits on the FEDEC-EXE Flex Channel.
			// The same goes for imm_u[7:3] i.e. zimm for the 3 CSRxI instructions.
		case ALUOP_CSRRW: // CSRRW
			csr_index = get_csr_index(input.imm_u.range(19, 8));
			output.alu_res = csr[csr_index];
                        set_csr_value(csr_index, input.rs1, CSR_OP_WR, input.imm_u.range(19, 18));
			break;
		case ALUOP_CSRRS: // CSRRS
			csr_index = get_csr_index(input.imm_u.range(19, 8));
			output.alu_res = csr[csr_index];
                        set_csr_value(csr_index, input.rs1, CSR_OP_SET, input.imm_u.range(19, 18));
			break;
		case ALUOP_CSRRC: // CSRRC
			csr_index = get_csr_index(input.imm_u.range(19, 8));
			output.alu_res = csr[csr_index];
                        set_csr_value(csr_index, input.rs1, CSR_OP_CLR, input.imm_u.range(19, 18));
			break;
		case ALUOP_CSRRWI: // CSRRWI
			csr_index = get_csr_index(input.imm_u.range(19, 8));
			output.alu_res = csr[csr_index];
                        set_csr_value(csr_index, input.imm_u.range(7, 3), CSR_OP_WR, input.imm_u.range(19, 18));
			break;
		case ALUOP_CSRRSI: // CSRRSI
			csr_index = get_csr_index(input.imm_u.range(19, 8));
			output.alu_res = csr[csr_index];
                        set_csr_value(csr_index, input.imm_u.range(7, 3), CSR_OP_SET, input.imm_u.range(19, 18));
			break;
		case ALUOP_CSRRCI: // CSRRCI
			csr_index = get_csr_index(input.imm_u.range(19, 8));
			output.alu_res = csr[csr_index];
                        set_csr_value(csr_index, input.imm_u.range(7, 3), CSR_OP_CLR, input.imm_u.range(19, 18));
			break;
#endif
		default: // ALUOP_NULL (do nothing)
			output.alu_res = (sc_bv<XLEN>)0;
			break;
		}


		// Forward
		reg_forward_t forward;
		forward.regfile_data = output.alu_res;
		forward.tag = output.tag;
		if (input.ld != NO_LOAD || input.st != NO_STORE)
			forward.ldst = true;
		else
			forward.ldst = false;

		fwd_exe.write(forward);

                if (!nop)
                    csr[MINSTRET_I]++;

		// Put
		dout.put(output);
		wait();
	}
}

/* Support functions */

// Sign extend immS.
sc_bv<XLEN> execute::sign_extend_imm_s(sc_bv<12> imm)
{
	if (imm.range(11, 11) == "1") // Extend with 1s
		return (sc_bv<20>("11111111111111111111"), imm);
	else // Extend with 0s
		return (sc_bv<20>("00000000000000000000"), imm);
}


#ifdef CSR_LOGIC
// Zero extends the zimm immediate field of CSRRWI, CSRRSI, CSRRCI
sc_bv<XLEN> execute::zero_ext_zimm(sc_bv<ZIMM_SIZE> zimm)
{
	return ("000000000000000000000000000", zimm);
}

// Return index given a csr address.
sc_uint<CSR_IDX_LEN> execute::get_csr_index(sc_bv<CSR_ADDR> csr_addr)
{
	switch ((sc_uint<CSR_ADDR>)csr_addr) {
        case USTATUS_A:
            return USTATUS_I;
        case MSTATUS_A:
            return MSTATUS_I;
        case MISA_A:
            return MISA_I;
        case MTVECT_A:
            return MTVECT_I;
        case MEPC_A:
            return MEPC_I;
        case MCAUSE_A:
            return MCAUSE_I;
        case MCYCLE_A:
            return MCYCLE_I;
        case MARCHID_A:
            return MARCHID_I;
        case MIMPID_A:
            return MIMPID_I;
        case MINSTRET_A:
            return MINSTRET_I;
        case MHARTID_A:
            return MHARTID_I;
	default:
		return 6; // TODO: this is not ideal. I default unsupported CSRs to MARCHID as it's not a critical register.
	}
}

// Set value of csr[csr_addr]
// TODO: respect unwritable fields, see manual for each individual implemented CSR.
// TODO: for now any bits of every register are fully readable/writeable.
// TODO: This must be changed in future implementations.
void execute::set_csr_value(sc_uint<CSR_IDX_LEN> csr_index, sc_bv<XLEN> rs1, sc_uint<LOG2_CSR_OP_NUM> operation, sc_bv<2> rw_permission)
{
    if (rw_permission != "11")
	switch(operation) {
        case CSR_OP_WR:
            csr[csr_index] = rs1;
            break;
        case CSR_OP_SET:
            csr[csr_index] |= (sc_uint<XLEN>)rs1;
            break;
        case CSR_OP_CLR:
            csr[csr_index] &= ~((sc_uint<XLEN>)rs1);
            break;
        default:
            break;
	}
}
#endif
