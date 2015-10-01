# the compiler to use
CC = clang++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall -std=c++11
  
#files to link:
LFLAGS = 
  
# the name to use for both the target source file, and the output file:
TARGET = winner
FILES = main.cpp framebuffer.cpp
  
all: $(TARGET)
  
$(TARGET): $(FILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(FILES) $(LFLAGS)