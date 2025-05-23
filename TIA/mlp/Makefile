CXX = g++
NVCC = nvcc
CXXFLAGS = -std=c++17 -O3
NVCCFLAGS = -std=c++17 -O3 -DUSE_CUDA
SRC = main.cpp mlp.cpp mlp.h
TARGET = main

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

cuda: $(SRC) mlp_cuda.cu
	$(NVCC) $(NVCCFLAGS) -o $(TARGET) $(SRC) mlp_cuda.cu

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)