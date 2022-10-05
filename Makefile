CC=gcc
INCDIRS=-I.
CFLAGS=-Wall -Wextra -g $(INCDIRS) $(OPT)

SRC=x.c y.c
OBJ=x.o y.o

TARGET=rv32iemu

all: $(TARGET)

$(TARGET): $(OBJ)
        $(CC) -o $@ $^

%.o:%.c
        $(CC) $(CFLAGS) -c -o $@ $^

clean:

