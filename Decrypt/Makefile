vpath %.c ../publicFuncLib

Objects = publib.o decrypt.o test.o

decrypt: $(Objects)
	gcc -o decrypt $(Objects) -lm

.PHONY: clean
clean:
	-rm decrypt *.o
