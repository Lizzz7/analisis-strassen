CC      = gcc
CFLAGS  = -O2 -std=c11 -Wall -Wextra -pedantic

BIN     = experimento
SRC     = main.c
DATA    = data.dat
PLOT    = plot.py

.PHONY: all run plot clean

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $@ $< 

run: $(BIN)
	./$(BIN) > $(DATA)

plot: run
	python3 $(PLOT) $(DATA)

clean:
	rm -f $(BIN) $(DATA) *_grafico.png
