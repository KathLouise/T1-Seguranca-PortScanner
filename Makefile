CFLAGS += -g

tudo: port_scanner.o
	gcc -o recon -lm $?
	rm -rf *.o

port_scanner.o: port_scanner.c

clean:
	rm -rf *.o

faxina: clean 
	rm -rf teste 
