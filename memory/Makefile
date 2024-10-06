CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS =


OBJS = $(SRCS:.c=.o)
TARGET = test_program
SRCS = $(wildcard *.c)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)