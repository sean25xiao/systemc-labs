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
			- [New] `set_dmi_allowed()` can tell the initator that the target is able to support the DMI. This is a hint to initiator that could speed up simulation becuase if DMI is failed, the initiator will not make the repeat calls to `get_direct_mem_tpr()`
		- Response Status: It is part of the transaction object and can be checked by the initiator when the transaction is complete
			- It should always be initialized to TLM_INCOMPLETE_RESPONSE, which indicates that the transaction has not reached the target or has not been executed by the target
			- It may be set by target
				- If the transaction is executed successfully, target should set it to tlm:L:TLM_OK_RESPONSE
				- If the transaction fails, target should choose between a predefined set of error response
			- Initiator should explicitly check the response from the transaction by using `is_response_error()` or `is_response_ok`; Also print the error type by `get_response_string`
	- It is recommended practice to set at least 8 of the 10 attributes explicitly


## Direct Memory Interface
- Purpose of DMI: speed up simulation by giving initiators a direct pointer to an area of memory in a target, thus bypassing the need to go through the transport interface for every single read and write transaction
- DMI uses both forward path and backward path
	- Forward Path: It permits function calls from initiator socket to target socket
	- Backward Path: It permits function calls from target socket to initiator socket
	- Forward DMI I/F: It lets an initiator request a direct memory pointer from a target
	- Backward DMI I/F: It lets a target invalidate a DMI pointer previously given to an initiator

- Initiator can get a new DMI pointer by calling `get_direct_mem_ptr()` along the forward path
	- `get_direct_mem_ptr()` needs to be implemented by the target and registered to the target socket
- Target can cancel the DMI (ie. wipe out the DMI pointer) by calling `invalidate_direct_mem_ptr()` from time to time
	- `invalidate_direct_mem_ptr()` needs to be implemented by the initiator and registered to the initiator socket

- `tlm::tlm_dmi dmi_data`: A data structure stored all the info for the initator for memory access, so that the initiator can have this DMI pointer for a particular address and for a particular mode of access
	- Modes of access: It includes read access, write access, or both
	- Target should decide whether or not it can grant the kind of access that being requested
	- `dmi_data.allow_read_write()`: The target grants read/write access whatever the mode of the request from the initiator
	- `dmi_data.set_dmi_ptr()`: It is the acutal direct memory pointer
		- This might not actually correspond to the requested address, since the target is free to gtrant any DMI region that encloses the requested address
		- It is better for the target to grant a region as large as possible
	- `dmi_data.set_start_address()` & `set_end_address()`: They describe the bounds of the DMI region from the point of view of the target
	- `dmi_data.set_read_latency()` & `dmi_data.set_write_latency`: They are estimates of the timing parameters for memory access

## Debug Transport Interface
- Purpose of Debug Transport I/F: give an initiator the ability to read or write memory in the target without causing any side-effects and without simulation time passing
- Debug Transport only uses forward path and has only one interface
	- The target must implement `transport_dbg` method and register this method to socket

- `transport_dbg`: A method that provided by target and is used to copy the given number of bytes for the initiator to debug
	- 
