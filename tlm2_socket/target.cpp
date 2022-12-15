#include "target.h"

Target::Target(sc_module_name _name)
	: sc_module(_name)
	, socket("socket")
	, LATENCY(20, SC_NS)
{
	cout << "Constructor of Target has been called" << endl;

	// register the defined b_transport method to socket
	socket.register_b_transport(this, &Target::b_transport);

	// register the defined get_direct_mem_ptr method to socket
	socket.register_get_direct_mem_ptr(this, &Target::get_direct_mem_ptr);
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
	/*if (adr >= sc_dt::uint64(SIZE) || byt != 0 || len > 4 || wid < len)
		SC_REPORT_ERROR("TLM-2", "Target does not support given generic payload transaction;");*/  // This is deprecated, we need setup different error response type for each failing case
	if (adr >= sc_dt::uint64(SIZE)) {
		trans.set_response_status( tlm::TLM_ADDRESS_ERROR_RESPONSE );
		SC_REPORT_ERROR("TLM-2", "Target received an address that is out-of-range;");
		return;
	}

	if (byt != 0) {
		trans.set_response_status( tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
		SC_REPORT_ERROR("TLM-2", "Target does not support byte enable;");
		return;
	}

	if (len > 4 || wid < len) {
		trans.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE);
		SC_REPORT_ERROR("TLM-2", "Target received a wrong length and streaming width;");
		return;
	}

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

	// Tell initiator that I (the target) can support the DMI by using this DMI hint attribute
	trans.set_dmi_allowed(true);

	wait(delay); // Blocking

	cout << "Target Memory: " << __FUNCTION__
		<< " trans = { " << (cmd ? 'W' : 'R') << ", "
		<< " }, data = " << mem[adr] << " at time " << sc_time_stamp()
		<< " delay = " << delay << endl;
}

// Detailed implementation for method b_transport
bool Target::get_direct_mem_ptr( tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data)
{
	dmi_data.allow_read_write();
	dmi_data.set_dmi_ptr( reinterpret_cast<unsigned char*>( &mem[0] ) );
	dmi_data.set_start_address( 0 );
	dmi_data.set_end_address( SIZE*4 - 1); // start ~ end address describes the bounds of the DMI region from the point of view of the target
	dmi_data.set_read_latency( LATENCY );
	dmi_data.set_write_latency ( LATENCY ); // estimates the timing parameters for memory access

	// Return true if the target can provide a DMI pointer, or false otherwise
	return true;
}
