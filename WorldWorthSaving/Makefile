CXX = g++

# Homebrew installation paths
INCLUDE_DIR = /opt/homebrew/include
LIB_DIR = /opt/homebrew/lib

CXXFLAGS = -std=c++17 -I$(INCLUDE_DIR) -Wall
LDFLAGS = -L$(LIB_DIR) -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Name of the binary
TARGET = WWS

# Source files in the src/ folder
SRCS = $(wildcard src/*.cpp)

# Output rule
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(SRCS) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGET)
