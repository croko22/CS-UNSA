CXX = g++
NVCC = nvcc
CXXFLAGS = -std=c++17 -O3
NVCCFLAGS = -std=c++17 -O3 -DUSE_CUDA
SRC = main.cpp
TARGET = main

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

cuda: $(SRC)
	$(NVCC) $(NVCCFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)