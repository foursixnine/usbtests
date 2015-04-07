all:
	g++ -o test test.cpp -I/usr/local/include -L/usr/local/lib -lusb-1.0
