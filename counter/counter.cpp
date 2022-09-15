#include "systemc.h"

SC_MODULE(counter) {
	sc_in_clk   clock;   // Clock input of the design
	sc_in<bool> reset;
	sc_in<bool> enable;
	sc_out<sc_uint<4>> counter_out;

	sc_uint<4> count;  // 4-bit
	void incr_count()
	{
		if (reset.read() == 1) {
			count = 0;
			counter_out.write(count);
		} else if (enable.read() == 1) {
			count = count + 1;
			counter_out.write(count);
			cout << "@" << sc_time_stamp() << "::Incremented Counter "
				<< counter_out.read() << endl;
		}

	}

	// Constructor
	SC_CTOR(counter)
	{
		cout << "Executing new counter" << endl;
		SC_METHOD(incr_count);
		sensitive << reset;
		sensitive << clock.pos();  // Positive edge trigged
	}

};
