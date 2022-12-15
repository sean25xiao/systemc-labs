#include "top.h"

Top::Top(sc_module_name _name)
	: sc_module(_name)
{
	// Instantiates one initiator and one memory
	p_initiator = new Initiator("myInitiator");
	p_target    = new Target("myMemory");
	cout << "The objects of Initiator and Target are created"
		<< " myInitiator's address is 0x" << std::hex << p_initiator
		<< " myMemory's address is 0x"    << std::hex << p_target    << endl;

	
	// Binds the initiator socket on the initiator to the target socket on the target memory
	p_initiator->socket.bind( p_target->socket );
	cout << "Initiator Socket on myInitiator has been binded to Target Socket on myMemory" << endl;
}
