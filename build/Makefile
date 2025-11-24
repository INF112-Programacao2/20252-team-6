# Compiladores
CXX = g++
CC = gcc

# Flags -> onde ta os headers 
CXXFLAGS = -Wall -Wextra -std=c++17 -I../include
CFLAGS = -Wall -Wextra -std=c11 -I../include

LDFLAGS =
TARGET = ../diarybetes.exe
SRCDIR = ../src
OBJDIR = ../obj

# Encontrar arquivos .cpp (na pasta src e na raiz) e .c (na src)
SOURCES_CPP = $(wildcard $(SRCDIR)/*.cpp) $(wildcard ../*.cpp)
SOURCES_C = $(wildcard $(SRCDIR)/*.c)

#gerando lista de objetos
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES_CPP)) \
          $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES_C))

HEADERS = $(wildcard ../include/*.hpp) $(wildcard ../include/*.h)


all: create_dirs $(TARGET)

# linkagem final
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJECTS) -o $(TARGET)

# Compilar arquivos C++ da pasta src
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compilar arquivos C da pasta src
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Criação de diretórios
create_dirs:
	@if not exist "..\obj" mkdir "..\obj"

# Limpeza
clean:
	@if exist "..\obj" rmdir /s /q "..\obj"
	@if exist "$(TARGET)" del /f /q "$(TARGET)"

.PHONY: all clean create_dirs