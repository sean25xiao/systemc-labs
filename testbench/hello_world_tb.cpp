#include "hello_world_tb.h"
#include "hello_world.h"

void hello_world_tb::run_test()
{
	hello_world hw("HELLO");
	hw.say_hello();
}
