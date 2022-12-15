#include <systemc.h>
//#include "counter.h"
#include "counter_tb.h"

int sc_main(int argc, char* argv[]) {

  counter_tb cnt_tb("Counter_Testbench");
  // Start the test
  cnt_tb.run_test();

  return(0);
}
