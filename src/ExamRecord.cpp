#include "../include/ExamRecord.hpp"
#include "../include/Patient.hpp"
#include "../include/HealthRecord.hpp"
#include<iostream>
#include<string>
#include<sqlite3.h>

ExamRecord::ExamRecord(Patient patient, std::string date, std::string hour, std::string nameExam, std::string result, std::string lab,std::string med)
    : HealthRecord(patient, date, hour), nameExam(nameExam), result(result), lab(lab), med(med){}

ExamRecord::~ExamRecord(){}

std::string ExamRecord::getName(){
    return nameExam;
}

std::string ExamRecord::getLab(){
    return lab;
}

std::string ExamRecord::getResult(){
    return result;
}

std::string ExamRecord::getMed(){
    return med;
}

void ExamRecord::registerDB(int id){
    sqlite3* db;
    sqlite3_stmt* stmt;
    sqlite3_stmt* stmt2;

    try{
        int rc=sqlite3_open("database.db",&db);
        if(rc != SQLITE_OK){
            std::cerr<<"Erro ao abrir database"<<std::endl;
            return;
        }
        const char* insert = "INSERT INTO RegistroSaude (Paciente, Hora, Data) VALUES (?,?,?)";
        if (sqlite3_prepare_v2(db, insert, -1, &stmt, nullptr) == SQLITE_OK) {
            //Vincular parâmetros as variaveis
            sqlite3_bind_int(stmt, 1, id);//id referente ao paciente, sera passado na main
            sqlite3_bind_text(stmt, 2, this->getHour().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, this->getDate().c_str(), -1, SQLITE_STATIC);
            if (sqlite3_step(stmt) == SQLITE_DONE) {
                std::cout << "Registro se saude inserido com sucesso com sucesso!" << std::endl;
                sqlite3_finalize(stmt);
                int idRecord = sqlite3_last_insert_rowid(db);//pega o id da ultima insercao
                const char* insert2 = "INSERT INTO RegistroExame (RegistroSaude, Nome, Medico, Laboratorio, Resultado) VALUES (?,?,?,?,?)";
                if (sqlite3_prepare_v2(db, insert2, -1, &stmt2, nullptr) == SQLITE_OK) {
                        // Use SQLITE_TRANSIENT para fazer uma cópia da string
                        sqlite3_bind_int(stmt2, 1, idRecord);
                        sqlite3_bind_text(stmt2, 2, this->getName().c_str(), -1, SQLITE_TRANSIENT);
                        sqlite3_bind_text(stmt2, 3, this->getMed().c_str(), -1, SQLITE_TRANSIENT);
                        sqlite3_bind_text(stmt2, 4, this->getLab().c_str(), -1, SQLITE_TRANSIENT);
                        sqlite3_bind_text(stmt2, 5, this->getResult().c_str(), -1, SQLITE_TRANSIENT);
                    if (sqlite3_step(stmt2) == SQLITE_DONE) {
                        std::cout << "Registro de exame inserido com sucesso com sucesso!" << std::endl;
                        sqlite3_finalize(stmt2);
                    } 
                    else {
                        std::cerr << "Erro ao executar inserção de exame: " << sqlite3_errmsg(db) << std::endl;
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


void ExamRecord::displayDetails() {
    std::cout << "Exame: " << nameExam << ", Resultado: " << result 
              << ", Laboratório: " << lab << std::endl;
}