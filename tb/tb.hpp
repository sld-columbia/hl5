/* Copyright 2017 Columbia University, SLD Group */

//
// tb.h - Robert Margelli
// Testbench header file.
//

#ifndef __TB__H
#define __TB__H

#include <time.h>
#include <systemc.h>
#include "cynw_flex_channels.h"

#include <esc.h>

#include "hl5_datatypes.hpp"

#include "globals.hpp"
#include "defines.hpp"

SC_MODULE(tb)
{
public:
	// Declaration of clock and reset parameters
	sc_in < bool > clk;
	sc_in < bool > rst;
	// End of simulation signal.
	sc_in < bool > program_end;
	// Fetch enable signal.
	sc_out < bool > fetch_en;
	// CPU Reset
	sc_out < bool > cpu_rst;
	// Entry point
	sc_out < unsigned > entry_point;

	// TODO: removeme
	// sc_in < bool > main_start;
	// sc_in < bool > main_end;

	// Instruction counters
	sc_in < long int > icount; 
	sc_in < long int > j_icount; 
	sc_in < long int > b_icount; 
	sc_in < long int > m_icount; 
	sc_in < long int > o_icount; 

	sc_uint<XLEN> *imem;
	sc_uint<XLEN> *dmem;

	SC_HAS_PROCESS(tb);
	tb(sc_module_name name, sc_uint<XLEN> imem[ICACHE_SIZE], sc_uint<XLEN> dmem[DCACHE_SIZE])
		: clk("clk")
		, rst("rst")
		, program_end("program_end")
		, fetch_en("fetch_en")
		, cpu_rst("cpu_rst")
		, entry_point("entry_point")
		// , main_start("main_start")
		// , main_end("main_end")
		, icount("icount")
		, j_icount("j_icount")
		, b_icount("b_icount")
		, m_icount("m_icount")
		, o_icount("o_icount")
		, imem(imem)
		, dmem(dmem)
	{
		SC_CTHREAD(source, clk.pos());
		reset_signal_is(rst, 0);
		SC_CTHREAD(sink, clk.pos());
		reset_signal_is(rst, 0);
	}
	void source();
	void sink();

        double exec_start;
	// double exec_main_start;
};

#endif
