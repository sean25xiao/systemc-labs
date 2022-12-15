${SYSTEMC_HOME} = /usr/local/systemc233

all:
	g++ -I. -I${SYSTEMC_HOME}/include -L. -L${SYSTEMC_HOME}/lib-linux64 -Wl,-rpath=${SYSTEMC_HOME}/lib-linux64 -o hello_world hello_world.cpp -lsystemc -lm
