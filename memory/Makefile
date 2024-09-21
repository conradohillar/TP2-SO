CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS =


OBJS = $(SRCS:.c=.o)
TARGET = test_program
SRCS = test_mm.c freeArray.c test_util.c syscall.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)