#ifndef __HELLO_WORLD_TB__
#define __HELLO_WORLD_TB__

#include <systemc.h>

SC_MODULE(hello_world_tb)
{
	SC_CTOR (hello_world_tb) {
	};

	void run_test();
};

#endif
