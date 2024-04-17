all: libkirk prxdecrypter main
	g++ --std=c++11 main.o bn.o ec.o kirk_engine.o SHA1.o AES.o amctrl.o PrxDecrypter.o -Wall -Os --static -o deceboot

main:
	g++ --std=c++11 -c main.cpp -Wall -Os
	
prxdecrypter:
	g++ -I. --std=c++11 -c Core/ELF/PrxDecrypter.cpp -Os
	
libkirk:
	gcc -c ext/libkirk/*.c -Os
	
clean:
	rm -f *.o deceboot.exe deceboot
