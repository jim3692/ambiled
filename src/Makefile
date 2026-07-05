# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS =

# Target executable name
TARGET = ambiled

# Get ImageMagick flags
CFLAGS += $(shell MagickWand-config --cflags --cppflags)
LDFLAGS += $(shell MagickWand-config --ldflags --libs)

# Find all source files
SRCS = $(wildcard *.c)

# Generate object files from source files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Rebuild everything
rebuild: clean all

.PHONY: all clean rebuild
