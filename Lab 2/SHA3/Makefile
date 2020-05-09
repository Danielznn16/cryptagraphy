main: SHA3.o
	g++ -o main SHA3.o -O2

SHA2.o: SHA3.cpp SHA3.h
	g++ -c SHA3.cpp -O2

clean:
	rm *.o
	rm main