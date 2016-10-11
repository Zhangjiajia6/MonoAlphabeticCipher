vpath %.c ../publicFuncLib

Objects = publib.o encrypt.o test.o

encrypt: $(Objects)
	gcc -o encrypt $(Objects)

.PHONY: clean
clean:
	-rm encrypt *.o
