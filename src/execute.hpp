/* Copyright 2017 Columbia University, SLD Group */

//
// execute.h - Robert Margelli
// execute stage header file.
//
// Division algorithm for DIV, DIVU, REM, REMU instructions. Division by zero
// and overflow semantics are compliant with the RISC-V specs (page 32).
//

#include <systemc.h>
#include "cynw_flex_channels.h"

#include "defines.hpp"
#include "globals.hpp"
#include "hl5_datatypes.hpp"

#include "syn_directives.hpp"

#ifndef __EXECUTE__H
#define __EXECUTE__H

#define BIT(_N) (1 << _N)

// Signed division quotient and remainder struct.
struct div_res_t{
	sc_int<XLEN> quotient;
	sc_int<XLEN> remainder;
};

// Unsigned division quotient and remainder struct.
struct u_div_res_t{
	sc_uint<XLEN> quotient;
	sc_uint<XLEN> remainder;
};

SC_MODULE(execute)
{
	// FlexChannel initiators
	get_initiator< de_out_t > din;
	put_initiator< exe_out_t > dout;

	// Forward
	sc_out< reg_forward_t > fwd_exe;

	// Clock and reset signals
	sc_in_clk clk;
	sc_in<bool> rst;

	// Thread prototype
	void execute_th(void);
	void perf_th(void);

	// Support functions
	sc_bv<XLEN> sign_extend_imm_s(sc_bv<12> imm);       // Sign extend the S-type immediate field.
	sc_bv<XLEN> zero_ext_zimm(sc_bv<ZIMM_SIZE> zimm);   // Zero extend the zimm field for CSRRxI instructions.
	sc_uint<CSR_IDX_LEN> get_csr_index(sc_bv<CSR_ADDR> csr_addr); // Get csr index given the 12-bit CSR address.
	void set_csr_value(sc_uint<CSR_IDX_LEN> csr_index, sc_bv<XLEN> rs1, sc_uint<LOG2_CSR_OP_NUM> operation, sc_bv<2> rw_permission);  // Perform requested CSR operation (write/set/clear).

	// Divider functions
	u_div_res_t udiv_func(sc_uint<XLEN> num, sc_uint<XLEN> den);
	div_res_t div_func(sc_int<XLEN> num, sc_int<XLEN> den);

	// Constructor
	SC_CTOR(execute)
		: din("din")
		, dout("dout")
		, fwd_exe("fwd_exe")
		, clk("clk")
		, rst("rst")
	{
		SC_CTHREAD(execute_th, clk.pos());
		reset_signal_is(rst, false);

		SC_CTHREAD(perf_th, clk.pos());
		reset_signal_is(rst, false);

		din.clk_rst(clk, rst);
		dout.clk_rst(clk, rst);
		HLS_FLATTEN_ARRAY(csr);
	}

	// Member variables
	de_out_t input;
	exe_out_t output;
	sc_uint<XLEN> csr[CSR_NUM]; // Control and status registers.
};


#endif
