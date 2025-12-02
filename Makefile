CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -g
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -g

SRCDIR = src
OBJDIR = obj
INCDIR = include

TARGET = diarybetes

all: check_sqlite3 $(TARGET)
	@echo "Compilacao concluida: $(TARGET)"

$(TARGET): main.o Person.o Patient.o Time.o HealthRecord.o \
           ConsultationRecord.o ExamRecord.o GlucoseRecord.o \
           Medication.o MedicationRecord.o MealPlan.o \
           DatabaseMethods.o sqlite3.o
	$(CXX) -o $(TARGET) main.o Person.o Patient.o Time.o HealthRecord.o \
           ConsultationRecord.o ExamRecord.o GlucoseRecord.o \
           Medication.o MedicationRecord.o MealPlan.o \
           DatabaseMethods.o sqlite3.o -g

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

Person.o: $(SRCDIR)/Person.cpp $(INCDIR)/Person.hpp
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/Person.cpp -o Person.o

Patient.o: $(SRCDIR)/Patient.cpp $(INCDIR)/Patient.hpp $(INCDIR)/Person.hpp
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/Patient.cpp -o Patient.o

Time.o: $(SRCDIR)/Time.cpp $(INCDIR)/Time.hpp
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/Time.cpp -o Time.o

HealthRecord.o: $(SRCDIR)/HealthRecord.cpp $(INCDIR)/HealthRecord.hpp $(INCDIR)/Patient.hpp
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/HealthRecord.cpp -o HealthRecord.o

ConsultationRecord.o: $(SRCDIR)/ConsultationRecord.cpp $(INCDIR)/ConsultationRecord.hpp $(INCDIR)/HealthRecord.hpp
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/ConsultationRecord.cpp -o ConsultationRecord.o

ExamRecord.o: $(SRCDIR)/ExamRecord.cpp $(INCDIR)/ExamRecord.hpp $(INCDIR)/HealthRecord.hpp
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/ExamRecord.cpp -o ExamRecord.o

GlucoseRecord.o: $(SRCDIR)/GlucoseRecord.cpp $(INCDIR)/GlucoseRecord.hpp $(INCDIR)/HealthRecord.hpp
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/GlucoseRecord.cpp -o GlucoseRecord.o

Medication.o: $(SRCDIR)/Medication.cpp $(INCDIR)/Medication.hpp
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/Medication.cpp -o Medication.o

MedicationRecord.o: $(SRCDIR)/MedicationRecord.cpp $(INCDIR)/MedicationRecord.hpp $(INCDIR)/HealthRecord.hpp $(INCDIR)/Medication.hpp
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/MedicationRecord.cpp -o MedicationRecord.o

MealPlan.o: $(SRCDIR)/MealPlan.cpp $(INCDIR)/MealPlan.hpp $(INCDIR)/Patient.hpp
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/MealPlan.cpp -o MealPlan.o

DatabaseMethods.o: $(SRCDIR)/DatabaseMethods.cpp $(INCDIR)/DatabaseMethods.hpp
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/DatabaseMethods.cpp -o DatabaseMethods.o

sqlite3.o: $(SRCDIR)/sqlite3.c $(INCDIR)/sqlite3.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/sqlite3.c -o sqlite3.o

check_sqlite3:
	@which sqlite3 > /dev/null 2>&1 || (echo "Instalando sqlite3..." && sudo apt-get update && sudo apt-get install -y sqlite3 libsqlite3-dev)

clean:
	rm -f *.o $(TARGET)
	@echo "Limpeza concluida."

.PHONY: all clean check_sqlite3
