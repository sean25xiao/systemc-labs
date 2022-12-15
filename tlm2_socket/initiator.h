#ifndef __INITIATOR_H__
#define __INITIATOR_H__

#define SC_INCLUDE_DYNAMIC_PROCESSES // simple socket spawn dynamic processes

#include <string>
#include <systemc.h>

using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include <tlm.h>
#include "tlm_utils/simple_initiator_socket.h"

class Initiator: public sc_module
{
public:
	Initiator(sc_module_name _name);

	// We need specify the parent module as first template arg
	// There are other args, but right now set as default
	// Ex. Width of the socket, the type of the transactions
	tlm_utils::simple_initiator_socket<Initiator> socket;

	// A thread process which can keep generating a stream of generic payload
	void generic_payload_generator();

private:
	int data;

};

#endif
