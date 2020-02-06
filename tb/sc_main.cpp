/* Copyright 2017 Columbia University, SLD Group */

//
// main.cpp - Robert Margelli
// Instantiates the top-level module (system) and
// launches simulation (sc_start).
//

#include "system.hpp"

TOP *top = NULL;

extern void esc_elaborate()
{
	top = new TOP("top");
}

extern void  esc_cleanup()
{
	delete top;
}

int sc_main(int argc, char *argv[])
{
	esc_initialize(argc, argv);
	esc_elaborate();

	sc_clock        clk("clk", 10, SC_NS);
	sc_signal<bool> rst("rst");

	top->clk(clk);
	top->rst(rst);

	rst.write(false);
	sc_start(51, SC_NS);
	rst.write(true);

	sc_start();

	esc_log_pass();

	return 0;
}
