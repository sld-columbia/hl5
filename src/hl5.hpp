/* Copyright 2017 Columbia University, SLD Group */

//
// hl5.h - Robert Margelli
// Header file of the hl5 CPU container.
// This module instantiates the stages and interconnects them.
//

#ifndef __HL5__H
#define __HL5__H

#include <systemc.h>
#include "cynw_flex_channels.h"

#include "hl5_datatypes.hpp"
#include "defines.hpp"
#include "globals.hpp"

#include "fedec_wrap.h"
#include "execute_wrap.h"
#include "memwb_wrap.h"

SC_MODULE(hl5)
{
public:
	// Declaration of clock and reset signals
	sc_in_clk      clk;
	sc_in < bool > rst;

	//End of simulation signal.
	sc_out < bool > program_end;

	// Fetch enable signal.
	sc_in < bool > fetch_en;

	// Entry point
	sc_in < unsigned > entry_point;

	// TODO: removeme
	// sc_out < bool > main_start;
	// sc_out < bool > main_end;

	// Instruction counters
	sc_out < long int > icount; 
	sc_out < long int > j_icount; 
	sc_out < long int > b_icount; 
	sc_out < long int > m_icount; 
	sc_out < long int > o_icount; 

	// Inter-stage Flex Channels.
	put_get_channel< de_out_t > de2exe_ch;
	put_get_channel< mem_out_t > wb2de_ch; // Writeback loop
	put_get_channel< exe_out_t > exe2mem_ch;

	// Forwarding
	sc_signal< reg_forward_t > fwd_exe_ch;

	SC_HAS_PROCESS(hl5);

	hl5(sc_module_name name,
	       sc_uint<XLEN> imem[ICACHE_SIZE],
	       sc_uint<XLEN> dmem[DCACHE_SIZE])
		: clk("clk")
		, rst("rst")
		, program_end("program_end")
		, fetch_en("fetch_en")
		// , main_start("main_start")
		// , main_end("main_end")
		, entry_point("entry_point")
		, de2exe_ch("de2exe_ch")
		, exe2mem_ch("exe2mem_ch")
		, wb2de_ch("wb2de_ch")
		, fwd_exe_ch("fwd_exe_ch")
		, fede("Fedec", imem)
		, exe("Execute")
		, mewb("Memory", dmem)
	{
		// FEDEC
		fede.clk(clk);
		fede.rst(rst);
		fede.dout(de2exe_ch);
		fede.feed_from_wb(wb2de_ch);
		fede.program_end(program_end);
		fede.fetch_en(fetch_en);
		fede.entry_point(entry_point);
		// fede.main_start(main_start);
		// fede.main_end(main_end);
		fede.fwd_exe(fwd_exe_ch);
		fede.icount(icount);
		fede.j_icount(j_icount);
		fede.b_icount(b_icount);
		fede.m_icount(m_icount);
		fede.o_icount(o_icount);

		// EXE
		exe.clk(clk);
		exe.rst(rst);
		exe.din(de2exe_ch);
		exe.dout(exe2mem_ch);
		exe.fwd_exe(fwd_exe_ch);

		// MEM
		mewb.clk(clk);
		mewb.rst(rst);
		mewb.din(exe2mem_ch);
		mewb.dout(wb2de_ch);
		mewb.fetch_en(fetch_en);
	}

	// Instantiate the modules
	fedec_wrapper fede;
	execute_wrapper exe;
	memwb_wrapper mewb;
};

#endif  // end __HL5__H
