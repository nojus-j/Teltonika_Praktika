CC      = gcc
CFLAGS  = -Wall -Wextra -O2
LDFLAGS = -lcurl -lcjson

SRC     = main.c net.c server.c location.c
OBJ     = $(SRC:.c=.o)
BIN     = speedtest

all: $(BIN)

$(BIN): $(OBJ)
    $(CC) $(OBJ) -o $(BIN) $(LDFLAGS)

%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

clean:
    rm -f $(OBJ) $(BIN)

.PHONY: all clean
