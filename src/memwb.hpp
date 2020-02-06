/* Copyright 2017 Columbia University, SLD Group */

// memwb.h - Robert Margelli
// memory+writeback stage header file.

#ifndef __MEMWB__H
#define __MEMWB__H

#include <systemc.h>

#include "cynw_flex_channels.h"

#include "defines.hpp"
#include "globals.hpp"
#include "syn_directives.hpp"
#include "hl5_datatypes.hpp"


SC_MODULE(memwb)
{
	// FlexChannel initiators
	get_initiator< exe_out_t > din;
	put_initiator< mem_out_t > dout;

	// Clock and reset signals
	sc_in_clk clk;
	sc_in<bool> rst;

	// Enable fetch
	sc_in<bool> fetch_en;   // Used to synchronize writeback with fetch at reset.

	// Instruction cache pointer to external memory
	sc_uint<XLEN> *dmem;

	// Thread prototype
	void memwb_th(void);
	// Function prototypes.
	sc_bv<XLEN> ext_sign_byte(sc_bv<BYTE> read_data);           // Sign extend byte read from memory. For LB
	sc_bv<XLEN> ext_unsign_byte(sc_bv<BYTE> read_data);         // Zero extend byte read from memory. For LBU
	sc_bv<XLEN> ext_sign_halfword(sc_bv<BYTE*2> read_data);     // Sign extend half-word read from memory. For LH
	sc_bv<XLEN> ext_unsign_halfword(sc_bv<BYTE*2> read_data);   // Zero extend half-word read from memory. For LHU

	// Constructor
	SC_HAS_PROCESS(memwb);
	memwb(sc_module_name name, sc_uint<XLEN> _dmem[DCACHE_SIZE])
		: din("din")
		, dout("dout")
		, clk("clk")
		, rst("rst")
		, fetch_en("fetch_en")
		, dmem(_dmem)
	{
		SC_CTHREAD(memwb_th, clk.pos());
		reset_signal_is(rst, false);
		din.clk_rst(clk, rst);
		dout.clk_rst(clk, rst);

		MAP_DCACHE;
	}

	// Member variables
	exe_out_t   input;
	mem_out_t   output;
	sc_bv<DATA_SIZE> mem_dout;  // Temporarily stores the value read from memory with a load instruction
};


#endif
