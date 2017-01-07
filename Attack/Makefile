vpath %.c ../publicFuncLib

attack: publib.o attack.o decrypt.o
	gcc -o attack attack.o publib.o decrypt.o -lm

.PHONY: clean
clean:
	-rm attack *.o
