# Nombre del archivo ejecutable
TARGET = galmacenamiento

# Compilador a utilizar
CXX = g++

# Opciones de compilación
CXXFLAGS = -std=c++17 -Wall

# Archivos fuente
SRCS = main.cpp GestorAlmacenamiento.cpp Page.cpp BufferPoolManager.cpp 

# Archivos de encabezado
HDRS = GestorAlmacenamiento.h Page.h BufferPoolManager.h

# Regla para compilar y generar el ejecutable
$(TARGET): $(SRCS) $(HDRS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

# Regla para limpiar los archivos generados
clean:
	rm -f $(TARGET)
