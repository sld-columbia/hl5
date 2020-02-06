/* Copyright 2017 Columbia University, SLD Group */

//
// tb.cpp - Robert Margelli
// Implementation of tb.hpp.
// In a normal run the source thread does nothing, while the sink waits for the
// program_end signal to be asserted and stops simulation
// (sc_stop).
//

#include "tb.hpp"

// Source thread
void tb::source()
{
	// Reset
	// Reset processor and disable fetch stage
	cpu_rst = 0;
	fetch_en = 0;
	wait();
	// De-assert reset
	SC_REPORT_INFO(sc_object::name(), "Done reset.");
	cpu_rst = 1;

	// Printing HLS config
	std::cout << "HLS config is: " << esc_argv(2) << std::endl;

	// Init imem
	// DBG_PROG;
	std::stringstream stm;
	stm << "Loading program from " << esc_argv(1) << "...";
	SC_REPORT_INFO(sc_object::name(), stm.str().c_str());

	std::ifstream dbg_prog;
	dbg_prog.open(esc_argv(1), std::ifstream::in);

	unsigned index;
	unsigned address;

        while (dbg_prog >> std::hex >> address) {
            index = address >> 2;
            if (index >= ICACHE_SIZE) {
                SC_REPORT_ERROR(sc_object::name(), "Program larger than memory size.");
                sc_stop();
                return;
            }
            dbg_prog >> std::hex >> imem[index];
            dmem[index] = imem[index];
        }


	dbg_prog.close();

	// Set entry point
	entry_point.write(0xfffffffc);

	wait();

	// Enable fetch stage
	SC_REPORT_INFO(sc_object::name(), "Fetch enable.");
	fetch_en = 1;

	// Record start time
	exec_start = esc_normalize_to_ps(sc_time_stamp()) / 1000;

	// do { wait(); } while (!main_start.read());

	// // Record start time
	// exec_main_start = esc_normalize_to_ps(sc_time_stamp()) / 1000;

	// Wait for global reset
	do { wait(); } while (true);
}

// Read all the expected values from the design
void tb::sink()
{
	// do { wait(); } while (!main_end.read());

	// // Record start time
	// double exec_main_end = esc_normalize_to_ps(sc_time_stamp()) / 1000;

	// Wait for end of simulation signal.
	do { wait(); } while (!program_end.read());

	// Record end time
	double exec_end = esc_normalize_to_ps(sc_time_stamp()) / 1000;

	// double elapsed_main_time_ns = exec_main_end - exec_main_start;
	double elapsed_time_ns = exec_end - exec_start;

	long icount_end, j_icount_end, b_icount_end, m_icount_end, o_icount_end;

	icount_end = icount.read();
	j_icount_end = j_icount.read();
	b_icount_end = b_icount.read();
	m_icount_end = m_icount.read();
	o_icount_end = o_icount.read();

	SC_REPORT_INFO(sc_object::name(), "Program complete.");

	std::cout << "Elapsed time: " << elapsed_time_ns << " ns." << std::endl;
	// std::cout << "Elapsed main time: " << elapsed_main_time_ns << " ns." << std::endl;
	std::cout << "INSTR TOT: " << icount_end << std::endl;
	std::cout << "   JUMP  : " << j_icount_end << std::endl;
	std::cout << "   BRANCH: " << b_icount_end << std::endl;
	std::cout << "   MEM   : " << m_icount_end << std::endl;
	std::cout << "   OTHER : " << o_icount_end << std::endl;

	// // Verify correctness of program
	// int is_check1_passed = 0, is_check2_passed = 0, is_check_failed = 0, is_test_passed = 0;

// 	for (int i = 0; i < DCACHE_SIZE - 1; i++) {
// #ifndef VERBOSE
// 		if (dmem[i] == 0x11111111 ||
// 		    dmem[i] == 0x22222222 ||
// 		    dmem[i] == 0x11223344 ||
// 		    dmem[i] == 0x88776655 ||
// 		    dmem[i] == 0x12345678 ||
// 		    dmem[i] == 0x87654321) {
// #endif
// 			std::cout << hex << "Address: " << i << "\tData: " 
// 				  << dmem[i] << std::endl;
			
// 			if (dmem[i] == 0x11223344)
// 				is_check1_passed = 1;
// 			else if (dmem[i] == 0x12345678)
// 				is_check2_passed = 1;
// 			else
// 				is_check_failed = 1;

// #ifndef VERBOSE
// 		}
// #endif
// 	}

	// if (is_check1_passed && is_check2_passed && !is_check_failed)
	// 	is_test_passed = 1;

	// Write report to file
	std::ofstream report;
	report.open(esc_argv(3), std::ofstream::out | std::ofstream::app);

	// report << "Program: " << esc_argv(1) << "\t Config: " << esc_argv(2) <<
	// 	"\t Time: " << elapsed_time_ns << "\t Main time: " << elapsed_main_time_ns << 
	//  	" ns.\tTest passed: " << is_test_passed << std::endl;

	report << "Program: " << esc_argv(1) << "\t Config: " << esc_argv(2) <<
		"\t Time: " << elapsed_time_ns // << "\t Main time: " << elapsed_main_time_ns
               << std::endl;

	report << "INSTR TOT: " << icount_end << std::endl;
	report << "   JUMP  : " << j_icount_end << std::endl;
	report << "   BRANCH: " << b_icount_end << std::endl;
	report << "   MEM   : " << m_icount_end << std::endl;
	report << "   OTHER : " << o_icount_end << std::endl;

	report.close();

    	esc_stop();
}

