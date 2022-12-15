#include "initiator.h"

SC_HAS_PROCESS(Initiator);

Initiator::Initiator(sc_module_name _name)
	: sc_module(_name) 
	, socket("socket")
{
	cout << "Constructor of Initiator has been called" << endl;

	// Register function generic_payload_generator as a thread process
	SC_THREAD(generic_payload_generator);
}

// Detailed Implementation of generic_payload_generator()
void Initiator::generic_payload_generator()
{
	tlm::tlm_generic_payload *p_trans = new tlm::tlm_generic_payload;
	sc_time delay = sc_time(10, SC_NS);

	for (int i = 32; i < 96; i += 4)
	{
		// Setup generic payload attributes
		tlm::tlm_command cmd = static_cast<tlm::tlm_command>(rand() % 2);
		if (cmd == tlm::TLM_WRITE_COMMAND) data = 0xFF000000 | i;

		p_trans->set_command( cmd );
		p_trans->set_address( i );
		p_trans->set_data_ptr( reinterpret_cast<unsigned char*>(&data) );
		p_trans->set_data_length( 4 );
		p_trans->set_streaming_width( 4 );
		p_trans->set_byte_enable_ptr( 0 );
		p_trans->set_dmi_allowed( false );
		p_trans->set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );

		cout << "Initiator: " << __FUNCTION__
			<< " trans = { " << (cmd ? 'W' : 'R') << ", " << std::hex << i
			<< " }, data = " << data << " at time " << sc_time_stamp()
			<< " delay = " << delay << endl;

		socket->b_transport( *p_trans, delay );

		if (p_trans->is_response_error())
			SC_REPORT_ERROR("TLM-2", "Response error from b_transport");
	}
}
