CC      = gcc
CFLAGS  = -O2 -std=c11 -Wall -Wextra -pedantic
LDFLAGS = -lm
BIN     = experimento
SRC     = main.c

.PHONY: all run clean

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN)
