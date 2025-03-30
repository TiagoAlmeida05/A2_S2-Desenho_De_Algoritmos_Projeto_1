# Compiler
CXX = g++
CXXFLAGS = -Wall -Wextra -g -std=c++11

# Source files
SRCS = Main.cpp driving.cpp 

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = Main

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up compiled files
clean:
	rm -f $(OBJS) $(TARGET)
