#include <systemc.h>

using namespace sc_core;
using namespace sc_dt;
using namespace std;

class Testbench: public sc_module
{
public:
	Testbench(sc_core::sc_module_name _name)
		: sc_module(_name)

	void run_test();
};
