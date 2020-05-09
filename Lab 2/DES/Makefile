main: DES.o
	g++ -o main DES.o -O2

te: te.cpp
	g++ -o te te.cpp -O2

DES.o: DES.cpp DES.h
	g++ -c DES.cpp -O2

clean:
	rm *.o
	rm main