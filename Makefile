CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -g
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -g

SRCDIR = src
OBJDIR = obj
INCDIR = include

TARGET = diarybetes

all: $(OBJDIR)/main.o $(OBJDIR)/Person.o $(OBJDIR)/Patient.o $(OBJDIR)/Time.o \
     $(OBJDIR)/HealthRecord.o $(OBJDIR)/ConsultationRecord.o $(OBJDIR)/ExamRecord.o \
     $(OBJDIR)/GlucoseRecord.o $(OBJDIR)/Medication.o $(OBJDIR)/MedicationRecord.o \
     $(OBJDIR)/MealPlan.o $(OBJDIR)/DatabaseMethods.o $(OBJDIR)/sqlite3.o
	$(CXX) -o $(TARGET) $(OBJDIR)/main.o $(OBJDIR)/Person.o $(OBJDIR)/Patient.o $(OBJDIR)/Time.o \
           $(OBJDIR)/HealthRecord.o $(OBJDIR)/ConsultationRecord.o $(OBJDIR)/ExamRecord.o \
           $(OBJDIR)/GlucoseRecord.o $(OBJDIR)/Medication.o $(OBJDIR)/MedicationRecord.o \
           $(OBJDIR)/MealPlan.o $(OBJDIR)/DatabaseMethods.o $(OBJDIR)/sqlite3.o -g

$(OBJDIR)/main.o: main.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c main.cpp -o $(OBJDIR)/main.o

$(OBJDIR)/Person.o: $(SRCDIR)/Person.cpp $(INCDIR)/Person.hpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/Person.cpp -o $(OBJDIR)/Person.o

$(OBJDIR)/Patient.o: $(SRCDIR)/Patient.cpp $(INCDIR)/Patient.hpp $(INCDIR)/Person.hpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/Patient.cpp -o $(OBJDIR)/Patient.o

$(OBJDIR)/Time.o: $(SRCDIR)/Time.cpp $(INCDIR)/Time.hpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/Time.cpp -o $(OBJDIR)/Time.o

$(OBJDIR)/HealthRecord.o: $(SRCDIR)/HealthRecord.cpp $(INCDIR)/HealthRecord.hpp $(INCDIR)/Patient.hpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/HealthRecord.cpp -o $(OBJDIR)/HealthRecord.o

$(OBJDIR)/ConsultationRecord.o: $(SRCDIR)/ConsultationRecord.cpp $(INCDIR)/ConsultationRecord.hpp $(INCDIR)/HealthRecord.hpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/ConsultationRecord.cpp -o $(OBJDIR)/ConsultationRecord.o

$(OBJDIR)/ExamRecord.o: $(SRCDIR)/ExamRecord.cpp $(INCDIR)/ExamRecord.hpp $(INCDIR)/HealthRecord.hpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/ExamRecord.cpp -o $(OBJDIR)/ExamRecord.o

$(OBJDIR)/GlucoseRecord.o: $(SRCDIR)/GlucoseRecord.cpp $(INCDIR)/GlucoseRecord.hpp $(INCDIR)/HealthRecord.hpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/GlucoseRecord.cpp -o $(OBJDIR)/GlucoseRecord.o

$(OBJDIR)/Medication.o: $(SRCDIR)/Medication.cpp $(INCDIR)/Medication.hpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/Medication.cpp -o $(OBJDIR)/Medication.o

$(OBJDIR)/MedicationRecord.o: $(SRCDIR)/MedicationRecord.cpp $(INCDIR)/MedicationRecord.hpp $(INCDIR)/HealthRecord.hpp $(INCDIR)/Medication.hpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/MedicationRecord.cpp -o $(OBJDIR)/MedicationRecord.o

$(OBJDIR)/MealPlan.o: $(SRCDIR)/MealPlan.cpp $(INCDIR)/MealPlan.hpp $(INCDIR)/Patient.hpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/MealPlan.cpp -o $(OBJDIR)/MealPlan.o

$(OBJDIR)/DatabaseMethods.o: $(SRCDIR)/DatabaseMethods.cpp $(INCDIR)/DatabaseMethods.hpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/DatabaseMethods.cpp -o $(OBJDIR)/DatabaseMethods.o

$(OBJDIR)/sqlite3.o: $(SRCDIR)/sqlite3.c $(INCDIR)/sqlite3.h
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SRCDIR)/sqlite3.c -o $(OBJDIR)/sqlite3.o

clean:
	rm -f $(OBJDIR)/*.o $(TARGET)
	@echo "Limpeza concluida."

.PHONY: all clean
