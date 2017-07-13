lsb: LSB.cpp
	g++ -g -c -std=c++17 LSB.cpp -o LSB.o -O2 -L/usr/X11R6/lib -lm -lpthread -lX11

main: 
	g++ -g -std=c++17 -o run test.cpp  LSB.o -L/usr/X11R6/lib -lm -lpthread -lX11

