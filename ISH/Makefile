ish : ish.o tools.o
	cc -o ish ish.o tools.o

ish.o : src/ish.c src/tools.h
	cc -c src/ish.c

tools.o : src/tools.c src/tools.h
	cc -c src/tools.c

clean :
	rm -f *.o ish

