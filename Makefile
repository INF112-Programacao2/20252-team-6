# Makefile pra Linux e Windows
ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
else
    UNAME_S := $(shell uname -s 2>/dev/null || echo "Linux")
    ifeq ($(UNAME_S),Linux)
        DETECTED_OS := Linux
    else ifeq ($(UNAME_S),Darwin)
        DETECTED_OS := Linux
    else
        # Tenta detectar via COMSPEC
        ifdef COMSPEC
            DETECTED_OS := Windows
        else
            DETECTED_OS := Linux
        endif
    endif
endif

# Configs por OS
ifeq ($(DETECTED_OS),Windows)
    RM := del /f /q
    RMDIR := rmdir /s /q 2>nul || rmdir /s /q
    MKDIR := if not exist
    MKDIR_CMD := @if not exist "$(OBJDIR)" mkdir "$(OBJDIR)"
    EXE_EXT := .exe
    PATHSEP := \\
    NULL := nul
else
    RM := rm -f
    RMDIR := rm -rf
    MKDIR := mkdir -p
    MKDIR_CMD := @mkdir -p $(OBJDIR)
    EXE_EXT :=
    PATHSEP := /
    NULL := /dev/null
endif

CXX = g++
CC = gcc

CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude -g
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -g
LDFLAGS = -g

SRCDIR = src
OBJDIR = obj
INCDIR = include

CPP_SOURCES_SRC = $(wildcard $(SRCDIR)/*.cpp)
CPP_SOURCES_ROOT = $(wildcard *.cpp)
C_SOURCES = $(wildcard $(SRCDIR)/*.c)

CPP_OBJECTS_SRC = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(CPP_SOURCES_SRC))
CPP_OBJECTS_ROOT = $(patsubst %.cpp,$(OBJDIR)/%.o,$(CPP_SOURCES_ROOT))
C_OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(C_SOURCES))
ALL_OBJECTS = $(CPP_OBJECTS_SRC) $(CPP_OBJECTS_ROOT) $(C_OBJECTS)

HEADERS = $(wildcard $(INCDIR)/*.hpp) $(wildcard $(INCDIR)/*.h)

TARGET = diarybetes$(EXE_EXT)

all: check_sqlite3 create_dirs $(TARGET)
	@echo "Compilacao concluida: $(TARGET)"
$(TARGET): $(ALL_OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(LDFLAGS) -o $(TARGET) $(ALL_OBJECTS)

# Regras .o
$(OBJDIR)/main.o: main.cpp $(HEADERS)
	@echo "Compilando main.cpp..."
	$(CXX) $(CXXFLAGS) -c main.cpp -o $(OBJDIR)/main.o

$(OBJDIR)/ConsultationRecord.o: $(SRCDIR)/ConsultationRecord.cpp $(INCDIR)/ConsultationRecord.hpp $(INCDIR)/HealthRecord.hpp $(INCDIR)/Patient.hpp
	@echo "Compilando ConsultationRecord.cpp..."
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/ConsultationRecord.cpp -o $(OBJDIR)/ConsultationRecord.o

$(OBJDIR)/DatabaseMethods.o: $(SRCDIR)/DatabaseMethods.cpp $(INCDIR)/DatabaseMethods.hpp
	@echo "Compilando DatabaseMethods.cpp..."
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/DatabaseMethods.cpp -o $(OBJDIR)/DatabaseMethods.o

$(OBJDIR)/ExamRecord.o: $(SRCDIR)/ExamRecord.cpp $(INCDIR)/ExamRecord.hpp $(INCDIR)/HealthRecord.hpp $(INCDIR)/Patient.hpp
	@echo "Compilando ExamRecord.cpp..."
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/ExamRecord.cpp -o $(OBJDIR)/ExamRecord.o

$(OBJDIR)/GlucoseRecord.o: $(SRCDIR)/GlucoseRecord.cpp $(INCDIR)/GlucoseRecord.hpp $(INCDIR)/HealthRecord.hpp $(INCDIR)/Patient.hpp
	@echo "Compilando GlucoseRecord.cpp..."
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/GlucoseRecord.cpp -o $(OBJDIR)/GlucoseRecord.o

$(OBJDIR)/HealthRecord.o: $(SRCDIR)/HealthRecord.cpp $(INCDIR)/HealthRecord.hpp $(INCDIR)/Patient.hpp
	@echo "Compilando HealthRecord.cpp..."
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/HealthRecord.cpp -o $(OBJDIR)/HealthRecord.o

$(OBJDIR)/MealPlan.o: $(SRCDIR)/MealPlan.cpp $(INCDIR)/MealPlan.hpp $(INCDIR)/Patient.hpp $(INCDIR)/sqlite3.h
	@echo "Compilando MealPlan.cpp..."
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/MealPlan.cpp -o $(OBJDIR)/MealPlan.o

$(OBJDIR)/Medication.o: $(SRCDIR)/Medication.cpp $(INCDIR)/Medication.hpp
	@echo "Compilando Medication.cpp..."
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/Medication.cpp -o $(OBJDIR)/Medication.o

$(OBJDIR)/MedicationRecord.o: $(SRCDIR)/MedicationRecord.cpp $(INCDIR)/MedicationRecord.hpp $(INCDIR)/HealthRecord.hpp $(INCDIR)/Medication.hpp $(INCDIR)/Patient.hpp
	@echo "Compilando MedicationRecord.cpp..."
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/MedicationRecord.cpp -o $(OBJDIR)/MedicationRecord.o

$(OBJDIR)/Patient.o: $(SRCDIR)/Patient.cpp $(INCDIR)/Patient.hpp $(INCDIR)/Person.hpp
	@echo "Compilando Patient.cpp..."
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/Patient.cpp -o $(OBJDIR)/Patient.o

$(OBJDIR)/Person.o: $(SRCDIR)/Person.cpp $(INCDIR)/Person.hpp
	@echo "Compilando Person.cpp..."
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/Person.cpp -o $(OBJDIR)/Person.o

$(OBJDIR)/Time.o: $(SRCDIR)/Time.cpp $(INCDIR)/Time.hpp
	@echo "Compilando Time.cpp..."
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/Time.cpp -o $(OBJDIR)/Time.o

$(OBJDIR)/sqlite3.o: $(SRCDIR)/sqlite3.c $(INCDIR)/sqlite3.h
	@echo "Compilando sqlite3.c..."
	$(CC) $(CFLAGS) -c $(SRCDIR)/sqlite3.c -o $(OBJDIR)/sqlite3.o

# Regras genericas extras 
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "Compilando $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.cpp
	@echo "Compilando $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Verifica/instala sqlite3
check_sqlite3:
ifeq ($(DETECTED_OS),Windows)
	@echo "Windows - pulando sqlite3"
else
	@which sqlite3 > /dev/null 2>&1 || (echo "Instalando sqlite3..." && sudo apt-get update && sudo apt-get install -y sqlite3 libsqlite3-dev)
	@pkg-config --exists sqlite3 || (echo "Instalando libsqlite3-dev..." && sudo apt-get install -y libsqlite3-dev)
endif

# Cria obj
create_dirs:
ifeq ($(DETECTED_OS),Windows)
	@if not exist "$(OBJDIR)" mkdir "$(OBJDIR)"
else
	@mkdir -p $(OBJDIR)
endif

# Limpeza
clean:
	@echo "Limpando arquivos compilados..."
ifeq ($(DETECTED_OS),Windows)
	@if exist "$(OBJDIR)" $(RMDIR) "$(OBJDIR)"
	@if exist "$(TARGET)" $(RM) "$(TARGET)"
	@echo "Limpeza concluida."
else
	@$(RMDIR) $(OBJDIR) 2>$(NULL) || true
	@$(RM) $(TARGET) 2>$(NULL) || true
	@echo "Limpeza concluida."
endif

# Recompila tudo
rebuild: clean all

# Ajuda
help:
	@echo "Makefile Diarybetes"
	@echo "OS: $(DETECTED_OS)"
	@echo ""
	@echo "Comandos:"
	@echo "  make       - Compila"
	@echo "  make clean - Limpa .o e executavel"
	@echo "  make rebuild - Limpa e recompila"
	@echo "  make help  - Mostra isso"

.PHONY: all clean create_dirs rebuild help check_sqlite3
