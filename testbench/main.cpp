#include <systemc.h>
#include "hello_world.h"
#include "hello_world_tb.h"

int sc_main(int argc, char* argv[]) {

  hello_world_tb hw_tb("HELLO_TB");
  // Print the hello world
  hw_tb.run_test();

  return(0);
}
