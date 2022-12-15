#ifndef __TOP_H__
#define __TOP_H__

#include <string>
#include "initiator.h"
#include "target.h"

class Top: public sc_module
{
public:
	Top(sc_module_name _name);

	Initiator *p_initiator;
	Target    *p_target;

};

#endif
