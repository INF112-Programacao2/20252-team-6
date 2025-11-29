#include "../include/ExamRecord.hpp"
#include "../include/Patient.hpp"
#include "../include/HealthRecord.hpp"
#include<iostream>
#include<string>
#include<sqlite3.h>
#include<iomanip>

ExamRecord::ExamRecord(const Patient& patient, std::string date, std::string hour, std::string nameExam,
     std::string result, std::string lab,std::string doctor)
    : HealthRecord(patient, date, hour), nameExam(nameExam), result(result), lab(lab), doctor(doctor){}

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

std::string ExamRecord::getDoctor(){
    return doctor;
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
                        sqlite3_bind_int(stmt2, 1, idRecord);
                        sqlite3_bind_text(stmt2, 2, this->getName().c_str(), -1, SQLITE_TRANSIENT);
                        sqlite3_bind_text(stmt2, 3, this->getDoctor().c_str(), -1, SQLITE_TRANSIENT);
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
    std::cout << "\nExame: " << nameExam << "\nPaciente: "<< getPatient().getName() <<"\nRequisitado por Dr(a):"<< doctor<< "\nResultado: " << result 
              << "\nLaboratório: " << lab << std::endl;
}

void ExamRecord::displayDetailsDB(int id) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    
    try {
        int rc = sqlite3_open("database.db", &db);
        if (rc != SQLITE_OK) {
            std::cerr << "Erro ao abrir database: " << sqlite3_errmsg(db) << std::endl;
            return;
        }
        const char* query = 
                "SELECT re.Nome, re.Medico, re.Laboratorio, re.Resultado, rs.Data, rs.Hora "
                "FROM RegistroExame re "
                "JOIN RegistroSaude rs ON re.RegistroSaude = rs.Id "
                "WHERE rs.Paciente = ? "
                "ORDER BY rs.Data DESC, rs.Hora DESC";
            
            if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_int(stmt, 1, id);
                
                //cabeçalho da tabela
                std::cout << "\n" << std::string(150, '-') << "\n";
                std::cout << "Registros de exame do paciente\n";
                std::cout << std::string(150, '-') << "\n";
                
                std::cout << std::left
                        << std::setw(20) << "Exame"
                        << "|" 
                        << std::setw(20) << "Medico"
                        << "|" 
                        << std::setw(20) << "Laboratorio"
                        << "|" 
                        << std::setw(70) << "Resultado"
                        << "|" 
                        << std::setw(8) << "Data"
                        << "|" 
                        << std::setw(5) << "Hora"
                        << "\n";
                std::cout << std::string(150, '-') << "\n";
        
            int recordCount = 0;
                
            //processar cada linha do resultado
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                std::string examName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                std::string doctor = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                std::string lab = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                std::string result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                std::string date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
                std::string time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
                    
                // Limitar o tamanho do resultado para caber na tabela
                if (result.length() > 67) {
                    result = result.substr(0, 67) + "...";
                }

                    std::cout << std::left
                    << std::setw(20) << examName
                        << "|"
                        << std::setw(20) << doctor
                        << "|"
                        << std::setw(20) << lab
                        << "|"
                        << std::setw(70) << result
                        << "|"
                        << std::setw(8) << date
                        << "|"
                        << std::setw(5) << time
                        << "\n";
                    
                    recordCount++;
                }
            std::cout << std::string(150, '-') << "\n";
            std::cout << "Total de registros encontrados: " << recordCount << "\n";
            std::cout << std::string(150, '-') << "\n";
            sqlite3_finalize(stmt);
        }
        else {
            std::cerr << "Erro ao preparar consulta: " << sqlite3_errmsg(db) << std::endl;
        }
        
        sqlite3_close(db);
    }
    catch(const std::exception& e){

    }
}