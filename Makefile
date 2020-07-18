SRC = main.c bmp.c drawing.c
OBJ = $(SRC:.c=.o)


CFLAGS = -g

SILLYFLAGS = -O3 -mavx2 -mfma -march=native

LDFLAGS = -lm

randwall: $(OBJ)
	gcc $^ $(CFLAGS) $(LDFLAGS) -o $@

%.o: %.c
	gcc -c $^ $(CFLAGS) -o $@


.PHONY: clean
clean:
	rm $(OBJ) randwall -v