#include <systemc.h>
#include "top.h"

int sc_main(int argc, char* argv[])
{
	Top top("myTop");
	//Top top;

	sc_start(100, SC_NS);
	return 0;
}
