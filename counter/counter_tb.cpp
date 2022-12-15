#include <systemc.h>
#include "counter.h"
#include "counter_tb.h"

void counter_tb::advance_clk(uint32_t cycles)
{
	for (uint32_t i = 0; i < cycles; i++)
	{
		clock = 0;
		//clock.write(0); // Both way are fine, "=" is overloaded to write()
		sc_start(1, SC_NS);
		clock = 1;
		//clock.write(1);
		sc_start(1, SC_NS);
	}
}

void counter_tb::run_test()
{
	sc_signal< sc_uint<4> > counter_out;

	my_counter my_counter("COUNTER");

	// Connect the DUT
	my_counter.clock(clock);
	my_counter.reset(reset);
    my_counter.enable(enable);
    my_counter.counter_out(counter_out);

	cout << "Counter Testbench Starts" << endl;

	//sc_start(1); // this is deprecated
	sc_start();

	// Open VCD File
	sc_trace_file *wf = sc_create_vcd_trace_file("counter");
	// Dump the desired signals
	sc_trace(wf, clock, "clk");
	sc_trace(wf, reset, "rst");
	sc_trace(wf, enable, "en");
	sc_trace(wf, counter_out, "counter_out");

	// Initialize all variables
	reset = 0;
	enable = 0;

	advance_clk(5);

	reset = 1;  // Assert reset
	cout << "@" << sc_time_stamp() << "::Asserting reset" << endl;
	advance_clk(5);

	reset = 0;  // De-Assert reset
	cout << "@" << sc_time_stamp() << "::De-Asserting reset" << endl;
	advance_clk(5);

	enable = 1; // Assert enable
	cout << "@" << sc_time_stamp() << "::Asserting enable" << endl;
	advance_clk(20);

	enable = 0; // Assert enable
	cout << "@" << sc_time_stamp() << "::De-Asserting enable" << endl;
	advance_clk(5);

	// Close trace VCD file
	cout << "@" << sc_time_stamp() << "::Ending Simulation" << endl;
	sc_close_vcd_trace_file(wf);
}

