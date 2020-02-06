/* Copyright 2017 Columbia University, SLD Group */

//
// system.h - Robert Margelli
// Top-level model header file.
// Instantiates the CPU, TB, IMEM, DMEM.
//

#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include <systemc.h>

#include "cynw_flex_channels.h"

#include "hl5_datatypes.hpp"
#include "defines.hpp"
#include "globals.hpp"

#include "hl5_wrap.h"

#include "tb.hpp"

SC_MODULE(TOP)
{
public:
	// Clock and reset
	sc_in<bool> clk;
	sc_in<bool> rst;

	// End of simulation signal.
	sc_signal < bool > program_end;
	// Fetch enable signal.
	sc_signal < bool > fetch_en;
	// CPU Reset
	sc_signal < bool > cpu_rst;
	// Entry point
	sc_signal < unsigned > entry_point;

	// TODO: removeme
	// sc_signal < bool > main_start;
	// sc_signal < bool > main_end;

	// Instruction counters
	sc_signal < long int > icount; 
	sc_signal < long int > j_icount; 
	sc_signal < long int > b_icount; 
	sc_signal < long int > m_icount; 
	sc_signal < long int > o_icount; 

	// Cache modeled as arryas
	sc_uint<XLEN> imem[ICACHE_SIZE];
	sc_uint<XLEN> dmem[DCACHE_SIZE];

	/* The testbench, DUT, IMEM and DMEM modules. */
	tb   *m_tb;
	hl5_wrapper *m_dut;

	SC_CTOR(TOP)
		: clk("clk")
		, rst("rst")
		, program_end("program_end")
		, fetch_en("fetch_en")
		, cpu_rst("cpu_rst")
		, entry_point("entry_point")
	{
		m_tb = new tb("tb", imem, dmem);

		m_dut = new hl5_wrapper("dut", imem, dmem);

		// Connect the design module
		m_dut->clk(clk);
		m_dut->rst(cpu_rst);
		m_dut->entry_point(entry_point);
		m_dut->program_end(program_end);
		m_dut->fetch_en(fetch_en);
		// m_dut->main_start(main_start);
		// m_dut->main_end(main_end);
		m_dut->icount(icount);
		m_dut->j_icount(j_icount);
		m_dut->b_icount(b_icount);
		m_dut->m_icount(m_icount);
		m_dut->o_icount(o_icount);

		// Connect the testbench
		m_tb->clk(clk);
		m_tb->rst(rst);
		m_tb->cpu_rst(cpu_rst);
		m_tb->entry_point(entry_point);
		m_tb->program_end(program_end);
		m_tb->fetch_en(fetch_en);
		// m_tb->main_start(main_start);
		// m_tb->main_end(main_end);
		m_tb->icount(icount);
		m_tb->j_icount(j_icount);
		m_tb->b_icount(b_icount);
		m_tb->m_icount(m_icount);
		m_tb->o_icount(o_icount);
	}

	~TOP()
	{
		delete m_tb;
		delete m_dut;
		delete imem;
		delete dmem;
	}

};

#endif // SYSTEM_H_INCLUDED

