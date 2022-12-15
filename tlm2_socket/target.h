#ifndef __TARGET_H__
#define __TARGET_H__

#define SC_INCLUDE_DYNAMIC_PROCESSES // simple socket spawn dynamic processes

#include <string>
#include <systemc.h>

using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include <tlm.h>
#include "tlm_utils/simple_target_socket.h"

class Target: public sc_module
{
public:
	Target(sc_core::sc_module_name _name);

	// We need specify the parent module as first template arg
	// There are other args, but right now set as default
	// Ex. Width of the socket, the type of the transactions
	// Width default is 32, Type default is tlm_generic_payload
	tlm_utils::simple_target_socket<Target> socket;

	enum { SIZE = 256 };

	// The initiator will communicate with the target using the blocking transport i/f
	// So the target needs to implement a single method named b_transport
	virtual void b_transport( tlm::tlm_generic_payload& trans, sc_time& delay);

	virtual bool get_direct_mem_ptr( tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data);

private:
	int mem[SIZE];

	const sc_time LATENCY;
};

#endif
