#include "initiator.h"

SC_HAS_PROCESS(Initiator);

Initiator::Initiator(sc_module_name _name)
	: sc_module(_name) 
	, socket("socket")
	, dmi_ptr_valid(false)
{
	cout << "Constructor of Initiator has been called" << endl;

	// Register function generic_payload_generator as a thread process
	SC_THREAD(generic_payload_generator);

	// Register invalidate_direct_mem_ptr() which is the DMI backforward path
	// It is used to wipe out any DMI pointers and requested by the target
	socket.register_invalidate_direct_mem_ptr(this, &Initiator::invalidate_direct_mem_ptr);
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

		// ************************************
		// DMI
		// ************************************
		// It reuses the same transaction object for both transport and DMI
		// which improves the efficiency of the simulation
		if (dmi_ptr_valid)
		{
			if ( cmd == tlm::TLM_READ_COMMAND) {
				assert( dmi_data.is_read_allowed() );
				memcpy(&data, dmi_data.get_dmi_ptr() + i, 4);
				wait( dmi_data.get_read_latency() );
			} // TLM_READ_COMMAND
			else if ( cmd == tlm::TLM_WRITE_COMMAND) {
				assert( dmi_data.is_write_allowed() );
				memcpy(dmi_data.get_dmi_ptr() + i, &data, 4);
				wait( dmi_data.get_write_latency() );
			}

			cout << "DMI   = { " << (cmd ? 'W' : 'R') << ", " << std::hex << i
				<< " } , data = " << data 
				<< " at time " << sc_time_stamp() << endl;
		}
		// ************************************
		// Transport
		// ************************************
		else {
			// If DMI is not enabled by target, just use transport
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

			if (p_trans->is_response_error()) {
				char txt[100];
				sprintf(txt, "Error from target b_transport, response status = %s", p_trans->get_response_string().c_str());
				SC_REPORT_ERROR("TLM-2", txt);
			}

			// Use DMI hint from the transaction to check wether or not the target supports DMI feature 
			if (p_trans->is_dmi_allowed()) {
				// If support
				dmi_data.init();
				dmi_ptr_valid = socket->get_direct_mem_ptr( *p_trans, dmi_data );
			}

		} // end of dmi_ptr_valid

	} // end of for loop

	// ************************************
	// Debug Transport Interface
	// ************************************
	cout << "Now request a debug transaction via Debug Transport Interface" << endl;

    p_trans->set_address(0);        // Set address
    p_trans->set_read();            // Set command
    p_trans->set_data_length(128);  // Set data length

    unsigned char* data = new unsigned char[128];
    p_trans->set_data_ptr(data);    // Set data pointer

    unsigned int n_bytes = socket->transport_dbg( *p_trans );

    for (unsigned int i = 0; i < n_bytes; i += 4)
    {
      cout << "mem[" << i << "] = "
           << *(reinterpret_cast<unsigned int*>( &data[i] )) << endl;
    }
}

void Initiator::invalidate_direct_mem_ptr(sc_dt::uint64 start_range, sc_dt::uint64 end_range)
{
	dmi_ptr_valid = false;
}
