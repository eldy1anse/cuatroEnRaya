# Nombre del compilador
CXX = g++

# Opciones del compilador
CXXFLAGS = -std=c++11 -pthread

# Archivo fuente
SRC = cliente.cpp

# Archivo objeto
OBJ = $(SRC:.cpp=.o)

# Archivo ejecutable
TARGET = cliente

# Regla principal para compilar y enlazar
all: $(TARGET)

# Regla para compilar el archivo objeto
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regla para compilar los archivos objeto
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para limpiar los archivos generados
clean:
	rm -f $(OBJ) $(TARGET)

# Regla para ejecutar el servidor
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run