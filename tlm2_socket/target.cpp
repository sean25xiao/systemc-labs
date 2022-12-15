#include "target.h"

Target::Target(sc_module_name _name)
	: sc_module(_name)
	, socket("socket")
{
	cout << "Constructor of Target has been called" << endl;

	// register the defined b_transport method to socket
	socket.register_b_transport(this, &Target::b_transport);
}

// Detailed implementation for method b_transport
void Target::b_transport( tlm::tlm_generic_payload& trans, sc_time& delay)
{
	tlm::tlm_command cmd = trans.get_command();
	sc_dt::uint64    adr = trans.get_address() / 4;
	unsigned char*   ptr = trans.get_data_ptr();
	unsigned int     len = trans.get_data_length();
	unsigned char*   byt = trans.get_byte_enable_ptr();
	unsigned int     wid = trans.get_streaming_width();
	// DMI Hint
	// Response Status
	// Byte Enable Length: Ignore here
	// Extension: Ignore here
	
	// Check the attribute to ensure that the initiator is not trying to use features
	// that the target cannot support
	// ie. Target memory does not support byte enables, streaming width, or burst transfers
	// ie. Also check that the address is out-of-range or not
	if (adr >= sc_dt::uint64(SIZE) || byt != 0 || len > 4 || wid < len)
		SC_REPORT_ERROR("TLM-2", "Target does not support given generic payload transaction;");

	// Implement the read and write command
	// by compying data to or from the data area in the initiator
	if (cmd == tlm::TLM_READ_COMMAND)
		memcpy(ptr, &mem[adr], len);
	else if (cmd == tlm::TLM_WRITE_COMMAND)
		memcpy(&mem[adr], ptr, len);

	// Set the response status attribute of the generic payload
	// to indicate the successful completion of the transaction
	// Note!! If not set, the default response status would indicate to the initiator that the transaction is incomplete
	trans.set_response_status( tlm::TLM_OK_RESPONSE );
	wait(delay);

	cout << "Target Memory: " << __FUNCTION__
		<< " trans = { " << (cmd ? 'W' : 'R') << ", "
		<< " }, data = " << mem[adr] << " at time " << sc_time_stamp()
		<< " delay = " << delay << endl;
}
