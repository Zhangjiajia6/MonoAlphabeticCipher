vpath %.c ../publicFuncLib

Objects = publib.o encrypt.o test.o

encrypt: $(Objects)
	gcc -o encrypt $(Objects) -lm

.PHONY: clean
clean:
	-rm encrypt *.o
