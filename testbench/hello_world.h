#ifndef __HELLO_WORLD__
#define __HELLO_WORLD__

#include <systemc.h>

// Hello_world is module name
SC_MODULE (hello_world) {

  SC_CTOR (hello_world) { // This is a constructor
    // Nothing in constructor
  };

  void say_hello();

};

#endif
