CXX = g++
CXXFLAGS = -Wall -std=c++17 -fopenmp
SRC = $(wildcard *.cpp)
OBJ = $(addprefix build/,$(SRC:.cpp=.o))
TARGET = build/main

all: build $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

build/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build

.PHONY: all cleans