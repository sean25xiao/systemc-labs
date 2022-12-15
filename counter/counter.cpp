#include "counter.h"

void my_counter::incr_count()
{
	if (reset.read() == 1) {
		count = 0;
		counter_out.write(count);
	} else if (enable.read() == 1) {
		count = count + 1;
		counter_out.write(count);
		cout << "@" << sc_time_stamp() << "::Incremented Counter "
			<< counter_out.read() << endl;
	}
}
