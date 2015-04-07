CXX = g++
CXXFLAGS = -std=c++11

SOURCES=test.cpp
OBJECTS=$(SOURCES:.cpp=.o)



all:
	$(CXX)  $(CXXFLAGS) test.cpp -o test -I/usr/local/include -L/usr/local/lib -lusb-1.0

clean:
	rm -rf *o $(EXECUTABLE)

