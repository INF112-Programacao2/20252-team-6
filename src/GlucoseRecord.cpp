#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <sqlite3.h>
#include "../include/Patient.hpp"
#include "../include/GlucoseRecord.hpp"
#include "../include/Time.hpp"

GlucoseRecord::GlucoseRecord(const Patient& patient, std::string date, Time hour, int glucoseLevel, bool fast)
    :HealthRecord(patient, date, hour), glucoseLevel(glucoseLevel), fast(fast){}

GlucoseRecord::~GlucoseRecord(){}

int GlucoseRecord::getGlucoseLevel(){
    return glucoseLevel;
}

bool GlucoseRecord::getFast(){
    return fast;
}

void GlucoseRecord::displayDetails(){
    std::cout << "Valores aproximados";
}

void GlucoseRecord::registerDB(int id){
    sqlite3* db;
    sqlite3_stmt* stmt;
    sqlite3_stmt* stmt2;

    try{
        int rc=sqlite3_open("database.db",&db);
        if(rc != SQLITE_OK){
            std::cerr<<"Erro ao abrir database"<<std::endl;
            return;
        }
        // Converte Time para string no formato HH:MM:SS
        std::ostringstream timeStr;
        timeStr << std::setfill('0') << std::setw(2) << this->getHour().getHour() << ":"
                << std::setfill('0') << std::setw(2) << this->getHour().getMinute() << ":"
                << std::setfill('0') << std::setw(2) << this->getHour().getSecond();

        const char* insert = "INSERT INTO RegistroSaude (Paciente, Data, Hora) VALUES (?,?,?)";
        if (sqlite3_prepare_v2(db, insert, -1, &stmt, nullptr) == SQLITE_OK) {
            //Vincular parâmetros as variaveis
            sqlite3_bind_int(stmt, 1, id);//id referente ao paciente, sera passado na main
            sqlite3_bind_text(stmt, 2, this->getDate().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, timeStr.str().c_str(), -1, SQLITE_TRANSIENT);
            if (sqlite3_step(stmt) == SQLITE_DONE) {
                std::cout << "Registro se saude inserido com sucesso com sucesso!" << std::endl;
                sqlite3_finalize(stmt);
                int idRecord = sqlite3_last_insert_rowid(db);//pega o id da ultima insercao
                const char* insert2 = "INSERT INTO RegistroGlicose (RegistroSaude, NivelGlicose, Jejum) VALUES (?,?,?)";
                if (sqlite3_prepare_v2(db, insert2, -1, &stmt2, nullptr) == SQLITE_OK) {
                        sqlite3_bind_int(stmt2, 1, idRecord);
                        sqlite3_bind_double(stmt2, 2, this->getGlucoseLevel());
                        bool fasting= this->getFast();
                        sqlite3_bind_blob(stmt2, 3, &fasting, sizeof(bool), SQLITE_TRANSIENT);
                    if (sqlite3_step(stmt2) == SQLITE_DONE) {
                        std::cout << "Registro de glicose inserido com sucesso com sucesso!" << std::endl;
                        sqlite3_finalize(stmt2);
                    } 
                    else {
                        std::cerr << "Erro ao executar inserção de glicoseexame: " << sqlite3_errmsg(db) << std::endl;
                        return;
                    }
                }
                else{
                    std::cerr << "Erro ao preparar SQL: " << sqlite3_errmsg(db) << std::endl;
                    return;
                }
            } 
            else {
                std::cerr << "Erro ao executar inserção do registro de saude: " << sqlite3_errmsg(db) << std::endl;
                sqlite3_finalize(stmt);
                return ;
            }
        }
        else {
            std::cerr << "Erro ao preparar SQL: " << sqlite3_errmsg(db) << std::endl;
            return;
        } 
        sqlite3_close(db);
    }
    catch(const std::exception& e){
        std::cerr << "Exceção: " << e.what() << std::endl;
    }
}