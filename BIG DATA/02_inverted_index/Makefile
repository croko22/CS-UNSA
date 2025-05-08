# Nombre del ejecutable
EXEC = inverted_index

# Archivo fuente principal
SRC = inverted_index.cpp

# Compilador
CXX = g++

# Opciones de compilación: C++17, OpenMP, optimización O3
CXXFLAGS = -std=c++17 -fopenmp -O3 -Wall -Wextra

# Regla por defecto: construye el ejecutable
all: $(EXEC)

# Regla para construir el ejecutable
$(EXEC): $(SRC)
	$(CXX) $(CXXFLAGS) $< -o $@

# Limpia archivos generados
clean:
	rm -f $(EXEC)

# Ejecutar el programa después de compilar
run: $(EXEC)
	./$(EXEC) files/