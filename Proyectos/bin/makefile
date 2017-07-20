lsb: 
	g++ -g -c -std=c++17 Steno/LSB.cpp -o Steno/LSB.o -O2 -L/usr/X11R6/lib -lm -lpthread -lX11

vs:
	g++ -g -c -std=c++17 VS/VS.cpp -o VS/VS.o -O2 -L/usr/X11R6/lib -lm -lpthread -lX11

#iface:
#	g++ -g -c -std=c++17 -o Interface/Interface.o Interface/Interface.cpp VS/VS.o -O2 -L/usr/X11R6/lib -lm -lpthread -lX11
utils:
	g++ -g -c -std=c++17 -o Utils/Utils.o Utils/Utils.cpp

testSteno: 
	g++ -g -std=c++17 -o runTestSteno testSteno.cpp  Steno/LSB.o Utils/Utils.o -O2 -L/usr/X11R6/lib -lm -lpthread -lX11

testVS:
	g++ -g -std=c++17 -o runTestVS testVS.cpp VS/VS.o Utils/Utils.o -O2 -L/usr/X11R6/lib -lm -lpthread -lX11

main:
	g++ -g -std=c++17 -o run main.cpp VS/VS.o Steno/LSB.o Utils/Utils.o -O2 -L/usr/X11R6/lib -lm -lpthread -lX11



