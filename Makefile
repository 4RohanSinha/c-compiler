CFILES := $(wildcard *.c)
scanner: $(wildcard *.c)
	gcc -o scanner -g $(CFILES)

clean:
	rm -f scanner *.o
