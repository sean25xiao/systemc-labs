#${SYSTEMC_HOME} = /usr/local/systemc233
SYSTEMC_HOME=/usr/local/systemc233

#all:
#	g++ -I. -I${SYSTEMC_HOME}/include -L. -L${SYSTEMC_HOME}/lib-linux64 -Wl,-rpath=${SYSTEMC_HOME}/lib-linux64 -o counter_tb counter_tb.cpp -lsystemc -lm

SYSTEMC_ARCH=linux64

LIB_DIRS=-L$(SYSTEMC_HOME)/lib-$(SYSTEMC_ARCH) -L.

RUN_TIME_SEARCH=-Wl,-rpath=$(SYSTEMC_HOME)/lib-$(SYSTEMC_ARCH)

INCLUDE_DIRS=-I. -I$(SYSTEMC_HOME)/include

LIBS=-lsystemc -lm

HEADERS=initiator.h target.h

SOURCES=main.cpp top.cpp target.cpp initiator.cpp

DEPENDENCIES=\
		Makefile\
		$(HEADERS)\
		$(SOURCES)

TESTS=main
all: $(TESTS)
	./$(TESTS)
	#@make cmp_result

# -g means provide a debug-able executable for debuger
$(TESTS): $(DEPENDENCIES)
	g++ -g -o $@ $(SOURCES) $(INCLUDE_DIRS) $(LIB_DIRS) $(RUN_TIME_SEARCH) $(LIBS)

#	g++ -I. -I${SYSTEMC_HOME}/include -L. -L${SYSTEMC_HOME}/lib-linux64 -Wl,-rpath=${SYSTEMC_HOME}/lib-linux64 -o counter_tb counter_tb.cpp -lsystemc -lm

clean:
	rm -rf $(TESTS) *.out *.dat
