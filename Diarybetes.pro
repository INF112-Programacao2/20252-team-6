QT += core gui sql charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = Diarybetes
TEMPLATE = app

# Diretórios
INCLUDEPATH += include
INCLUDEPATH += gui

# Headers existentes
HEADERS += \
    include/Person.hpp \
    include/Patient.hpp \
    include/Time.hpp \
    include/Medication.hpp \
    include/MedicationRecord.hpp \
    include/HealthRecord.hpp \
    include/ExamRecord.hpp \
    include/ConsultationRecord.hpp \
    include/GlucoseRecord.hpp \
    include/MealPlan.hpp \
    include/DatabaseMethods.hpp \
    gui/MainWindow.hpp

# Sources existentes
SOURCES += \
    src/Person.cpp \
    src/Patient.cpp \
    src/Time.cpp \
    src/Medication.cpp \
    src/MedicationRecord.cpp \
    src/HealthRecord.cpp \
    src/ExamRecord.cpp \
    src/ConsultationRecord.cpp \
    src/GlucoseRecord.cpp \
    src/MealPlan.cpp \
    src/DatabaseMethods.cpp \
    src/sqlite3.c \
    gui/main_gui.cpp \
    gui/MainWindow.cpp

# Forms
FORMS += \
    ui/forms/MainWindow.ui

# Flags
QMAKE_CXXFLAGS += -Wall -Wextra

# Output
DESTDIR = $$PWD/bin
OBJECTS_DIR = $$PWD/obj
MOC_DIR = $$PWD/moc
UI_DIR = $$PWD/ui_generated

