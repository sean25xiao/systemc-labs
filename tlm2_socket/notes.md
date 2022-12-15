- Initiator: a module that initiates new transactions
- Target: a module that responds to transactions initiated by other modules
- Interconnect: a module that merely forwards transactions without modifying their contents

- Transaction: a data structure (a C++ object) passed between initiators and targets using function calls

- Sockets: is a set of ports that used to pass transactions between initiators and targets
	- The socket encapsulates everything we need for two-way communication between modules
	- An initiator sends transactions out through an initiator socket
		- Initiator socket is actually an `sc_port` that has an `sc_export` on the side
	- An target receives incoming transactions through a target socket
		- Target socket is actually an `sc_export` that has an `sc_port` on the side
	- `socket.bind()` operator in the socket class binds port-to-export in both directions

- What type of sockets does initiator, target, and interconnect has?
	- Initiator has initiator socket
	- Target has target socket
	- Interconnect has both initiator socket and target socket

- Classes in the tlm_utils namespace exist for convenience and productivity
- Classes in the tlm namespace are actually essential for interoperability
- tlm_utils
	- tlm_utils::simple_initiator_socket<PARENT-MODULE> derived from tlm_initiator_socket
	- tlm_utils::simple_target_socket<PARENT-MODULE> derived from tlm_target_socket

- `tlm::tlm_generic_payload`
	- Each generic payload transaction has a standard set of attributes
		- Commnad: Supports two commands, read and write
		- Address: It is the lowest address value to which data is to be read or written
		- Data Pointer: Points to a data buffer within the initiator
			- Although default value is 0, but 0 is not permitted
		- Data Length: Gives the length of the data array in bytes
			- Although default value is 0, but 0 is not permitted
		- (The data will be copied from the data array to the target in write command, and from the target to the data array in read command)
		- Byte Enable Pointer
			- Set to 0 for unuse
		- Byte Enables Length
			- Do not set this if Byte Enable Pointer is set to 0 (ie. unused)
		- Streaming Width: Specifies the width of a streaming burst
			- For non-streaming transaction, the streaming width should equal to the data length
			- Although default value is 0, but 0 is not permitted
		- DMI Allowed: 
			- It should always be initialized to FALSE
			- It may be set by the target to indicate that it supports the direct memory interface
		- Response Status
			- It should always be initialized to TLM_INCOMPLETE_RESPONSE
			- It may be set by target
	- It is recommended practice to set at least 8 of the 10 attributes explicitly
