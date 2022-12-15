#ifndef __MY_COUNTER__
#define __MY_COUNTER__

#include <systemc.h>

SC_MODULE(my_counter)
{
	sc_in_clk   clock;   // Clock input of the design
	sc_in<bool> reset;
	sc_in<bool> enable;
	sc_out<sc_uint<4>> counter_out;

	sc_uint<4> count;  // 4-bit

	// Constructor
	SC_CTOR(my_counter) {
		cout << "Executing new counter" << endl;
		SC_METHOD(incr_count);
		sensitive << reset;
		sensitive << clock.pos();  // Positive edge trigged
	}

	void incr_count();
};

#endif
