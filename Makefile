# Variables
CC = g++
CFLAGS = -Wall -Wextra -std=c++11
TARGET = bin/cpu_emulator.exe

# Source files
SRCS = src/main.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Rule to build the target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to build object files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(TARGET) $(OBJS)

# Phony targets
.PHONY: all clean
