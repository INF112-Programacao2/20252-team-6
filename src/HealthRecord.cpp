#include "../include/HealthRecord.hpp"
#include "../include/Patient.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <sqlite3.h>

HealthRecord::HealthRecord(const Patient& patient, std::string date, Time hour)
    : patient(patient), date(date), hour(hour){}

HealthRecord::~HealthRecord(){}

void HealthRecord::registerDB(int id){
    //conectando ao banco de  dados
    sqlite3* db;
    sqlite3_stmt* stmt;

    int rc=sqlite3_open("database.db",&db);
    if (rc) {
        std::cerr << "Erro ao abrir banco: " << sqlite3_errmsg(db) << std::endl;
    }
    
    // Converte Time para string no formato HH:MM:SS
    std::ostringstream timeStr;
    timeStr << std::setfill('0') << std::setw(2) << hour.getHour() << ":"
            << std::setfill('0') << std::setw(2) << hour.getMinute() << ":"
            << std::setfill('0') << std::setw(2) << hour.getSecond();
    
    const char* insert = "INSERT INTO RegistroSaude (Paciente, Data, Hora) VALUES (?,?,?)";
     if (sqlite3_prepare_v2(db, insert, -1, &stmt, nullptr) == SQLITE_OK) {
        //Vincular parâmetros as variaveis
        sqlite3_bind_int(stmt, 1, id);//id referente ao paciente, sera passado na main
        sqlite3_bind_text(stmt, 2, date.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, timeStr.str().c_str(), -1, SQLITE_STATIC);
        // Executar inserção
        if (sqlite3_step(stmt) == SQLITE_DONE) {
            std::cout << "Registro de saude atulizado com sucesso registrado com sucesso com sucesso!" << std::endl;
        } else {
            std::cerr << "Erro ao executar inserção: " << sqlite3_errmsg(db) << std::endl;
        }
        
        // Finalizar statement
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Erro ao preparar SQL: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_close(db);
}

std::string HealthRecord::getDate() const {
    return date;
}

Time HealthRecord::getHour() const {
    return hour;
}

std::string HealthRecord::getHourString() const {
    std::ostringstream timeStr;
    timeStr << std::setfill('0') << std::setw(2) << hour.getHour() << ":"
            << std::setfill('0') << std::setw(2) << hour.getMinute() << ":"
            << std::setfill('0') << std::setw(2) << hour.getSecond();
    return timeStr.str();
}

void HealthRecord::displayDetails() {
    std::cout << "Data: " << date << ", Hora: ";
    hour.displayTime24();
}

void HealthRecord::displayDetailsDB(int id) {
    std::cout << "Data: " << date << ", Hora: ";
    hour.displayTime24();
}

Patient HealthRecord::getPatient() const {
    return patient;
}