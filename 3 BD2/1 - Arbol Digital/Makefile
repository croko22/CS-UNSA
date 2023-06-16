# Nombre del archivo ejecutable
TARGET = dtree

# Compilador a utilizar
CXX = g++

# Opciones de compilación
CXXFLAGS = -std=c++17 -Wall

# Archivos fuente
SRCS = main.cpp DTree.cpp

# Archivos de encabezado
HDRS = DTree.h Node.h

# Regla para compilar y generar el ejecutable
$(TARGET): $(SRCS) $(HDRS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

# Regla para limpiar los archivos generados
clean:
	rm -f $(TARGET)
