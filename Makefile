CC = g++
CFLAGS = -Wall -Wextra -std=c++11 -Os -ffunction-sections -fdata-sections -Wl,--gc-sections
TARGET = bin/armstrong
ASSEMBLER_TARGET = bin/assembler

SRCS = src/main.cpp
ASSEMBLER_SRCS = src/assembler.cpp

OBJS = $(SRCS:.cpp=.o)
ASSEMBLER_OBJS = $(ASSEMBLER_SRCS:.cpp=.o)

all: $(TARGET) $(ASSEMBLER_TARGET)
assembler: $(ASSEMBLER_TARGET)
armstrong: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -Isrc/include/SDL2 -o $(TARGET) $(OBJS) -Lsrc/lib -lmingw32 -lSDL2main -lSDL2 -luser32
	strip --strip-all $(TARGET).exe -o $(TARGET)stripped.exe

$(ASSEMBLER_TARGET): $(ASSEMBLER_OBJS)
	$(CC) $(CFLAGS) -o $(ASSEMBLER_TARGET) $(ASSEMBLER_OBJS)

src/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

cleanass:
	del src\assembler.o
	del bin\assembler.exe

clean:
	del src\assembler.o
	del src\main.o
	del bin\armstrong.exe
	del bin\armstrongstripped.exe
	del bin\assembler.exe

FORCE:
