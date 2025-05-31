# Enhanced Makefile for Raspberry Pi CPU & Memory Monitor

CC = gcc
CFLAGS = -Wall -O2
SRC = src/c/main.c
OBJ = src/c/main.o
TARGET = src/c/main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)