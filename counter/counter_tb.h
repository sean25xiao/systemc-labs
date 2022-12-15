#ifndef __COUNTER_TB__
#define __COUNTER_TB__

#include <systemc.h>

SC_MODULE(counter_tb)
{
	sc_signal<bool>   clock;
	sc_signal<bool>   reset;
	sc_signal<bool>   enable;

	SC_CTOR(counter_tb) {};

	void advance_clk(uint32_t cycles);
	void run_test();
};

#endif
