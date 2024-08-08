# Variables
CC = g++
CFLAGS = -Wall -Wextra -std=c++11
TARGET = bin/armstrong
ASSEMBLER_TARGET = bin/assembler

# Source files
SRCS = src/main.cpp
ASSEMBLER_SRCS = src/assembler.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)
ASSEMBLER_OBJS = $(ASSEMBLER_SRCS:.cpp=.o)

# Default target
all: $(TARGET) $(ASSEMBLER_TARGET)

# Rule to build the main target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to build the assembler target
$(ASSEMBLER_TARGET): $(ASSEMBLER_OBJS)
	$(CC) $(CFLAGS) -o $(ASSEMBLER_TARGET) $(ASSEMBLER_OBJS)

# Rule to build object files from source files in src/
src/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

FORCE:
