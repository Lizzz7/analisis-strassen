CC      = gcc
CFLAGS  = -O2 -std=c11 -Wall -Wextra -pedantic
LDFLAGS = -lm

BIN     = experimento
SRC     = main.c
DATA    = data.dat
PLOT    = plot.gp

.PHONY: all run plot clean

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

run: $(BIN)
	./$(BIN) > $(DATA)

plot: run
	gnuplot -e "set title 'Clasico vs Strassen'; \
	set xlabel 'n'; \
	set ylabel 'Tiempo (us)'; \
	set grid; \
	plot '$(DATA)' using 1:2 with linespoints title 'Clasico', \
		'$(DATA)' using 1:3 with linespoints title 'Strassen'; \
	pause -1"

clean:
	rm -f $(BIN) $(DATA)
