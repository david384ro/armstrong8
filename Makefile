# Variables
CC = g++
CFLAGS = -Wall -Wextra -std=c++11
TARGET = bin/armstrong

# Source files
SRCS = src/main.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Rule to build the target
$(TARGET): FORCE $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to build object files
%.o: %.cpp FORCE
	$(CC) $(CFLAGS) -c $< -o $@

FORCE: