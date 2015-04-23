MAX_URB?=256
CXXFLAGS += -std=c++11
USERFLAGS += -DMAX_URB=$(MAX_URB)

SOURCES=test.cpp
OBJECTS=$(SOURCES:.cpp=.o)

all: libusbtest peformance

libusbtest:
	$(CXX)  $(CXXFLAGS) test.cpp -o libusbtest -I/usr/local/include -L/usr/local/lib -lusb-1.0

peformance:
	$(CXX)  $(CXXFLAGS) peformance_test.cpp -o peformance_test -I/usr/local/include -L/usr/local/lib -lusb-1.0

peformance_transfers:

	$(CXX)  $(CXXFLAGS) $(USERFLAGS) mass_command_test.cpp -o mass_command_test_$(MAX_URB) -I/usr/local/include -L/usr/local/lib -lusb-1.0

clean:
	rm -rf *o peformance_test libusbtest mass_command_test_*

