#include <iostream>
#include <iomanip>
#include <string>
#include <sqlite3.h>
#include "../include/DatabaseMethods.hpp"

DatabaseMethods::DatabaseMethods(){}
DatabaseMethods::~DatabaseMethods(){}

void DatabaseMethods::displayDetailsExamRecordDB(int id) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    
    try {
        int rc = sqlite3_open("database.db", &db);
        if (rc != SQLITE_OK) {
            std::cerr << "Erro ao abrir database: " << sqlite3_errmsg(db) << std::endl;
            return;
        }
        const char* query = 
                "SELECT re.Nome, re.Medico, re.Laboratorio, re.Resultado, rs.Hora, rs.Data "
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
                        << std::setw(10) << "Data"
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
                std::string time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
                std::string date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
                    
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

void DatabaseMethods::displayDetailsConsultationRecordDB(int id){
    sqlite3* db;
    sqlite3_stmt* stmt;
    
    try {
        int rc = sqlite3_open("database.db", &db);
        if (rc != SQLITE_OK) {
            std::cerr << "Erro ao abrir database: " << sqlite3_errmsg(db) << std::endl;
            return;
        }
        const char* query = 
                "SELECT rc.Medico, rc.Especialidade, rc.Local, rc.Descricao, rs.Hora, rs.Data "
                "FROM RegistroConsulta rc "
                "JOIN RegistroSaude rs ON rc.RegistroSaude = rs.Id "
                "WHERE rs.Paciente = ? "
                "ORDER BY rs.Data DESC, rs.Hora DESC";
            
            if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_int(stmt, 1, id);
                
                //cabeçalho da tabela
                std::cout << "\n" << std::string(150, '-') << "\n";
                std::cout << "Registros de consulta do paciente\n";
                std::cout << std::string(150, '-') << "\n";
                
                std::cout << std::left
                        << std::setw(20) << "Medico"
                        << "|" 
                        << std::setw(20) << "Especialidade"
                        << "|" 
                        << std::setw(20) << "Local"
                        << "|" 
                        << std::setw(70) << "Descricao"
                        << "|" 
                        << std::setw(10) << "Data"
                        << "|" 
                        << std::setw(5) << "Hora"
                        << "\n";
                std::cout << std::string(150, '-') << "\n";
        
            int recordCount = 0;
                
            //processar cada linha do resultado
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                std::string doctor = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                std::string specialty = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                std::string local = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                std::string descrition = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                std::string time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
                std::string date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
                    
                // Limitar o tamanho do resultado para caber na tabela
                if (descrition.length() > 67) {
                    descrition = descrition.substr(0, 67) + "...";
                }

                    std::cout << std::left
                    << std::setw(20) << doctor
                        << "|"
                        << std::setw(20) << specialty
                        << "|"
                        << std::setw(20) << local
                        << "|"
                        << std::setw(70) << descrition
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

void DatabaseMethods::displayDetailsGlucoseRecordDB(int id) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    
    try {
        int rc = sqlite3_open("database.db", &db);
        if (rc != SQLITE_OK) {
            std::cerr << "Erro ao abrir database: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        const char* query = 
            "SELECT rg.NivelGlicose, rg.Jejum, rs.Data, rs.Hora "
            "FROM RegistroGlicose rg "
            "JOIN RegistroSaude rs ON rg.RegistroSaude = rs.Id "
            "WHERE rs.Paciente = ? "
            "ORDER BY rs.Data DESC, rs.Hora DESC";
            
        if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, id);
            
            // Cabeçalho da tabela
            std::cout << "\n" << std::string(38, '-') << "\n";
            std::cout << "Registros de glicose" << "\n";
            std::cout << std::string(38, '-') << "\n";
            
            std::cout << std::left
                      << std::setw(12) << "Glicose"
                      << "|"
                      << std::setw(8) << "Jejum"
                      << "|"
                      << std::setw(10) << "Data"
                      << "|"
                      << std::setw(5) << "Hora"
                      << "\n";
            std::cout << std::string(38, '-') << "\n";
    
            int recordCount = 0;
            
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // NivelGlicose (REAL) - coluna 0
                double glucoseLevel = sqlite3_column_double(stmt, 0);
                
                // Jejum (BLOB) - coluna 1 - TRATAMENTO ESPECIAL PARA BLOB
                bool fasting = false;
                std::string fastingStr = "Nao";
                
                if (sqlite3_column_type(stmt, 1) == SQLITE_BLOB) {
                    const void* blobData = sqlite3_column_blob(stmt, 1);
                    int blobSize = sqlite3_column_bytes(stmt, 1);
                    
                    if (blobData != nullptr && blobSize >= sizeof(bool)) {
                        // Converter BLOB para bool
                        fasting = *static_cast<const bool*>(blobData);
                        fastingStr = fasting ? "Sim" : "Nao";
                    } else {
                        fastingStr = "Erro";
                    }
                } else if (sqlite3_column_type(stmt, 1) == SQLITE_INTEGER) {
                    // Fallback: se for INTEGER em vez de BLOB
                    int fastingInt = sqlite3_column_int(stmt, 1);
                    fastingStr = (fastingInt != 0) ? "Sim" : "Nao";
                } else {
                    fastingStr = "Inválido";
                }
                
                // Data e Hora - colunas 2 e 3
                std::string data = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                std::string hora = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                
                // Exibir na tabela
                std::cout << std::left
                          << std::setw(12) << glucoseLevel
                          << "|"
                          << std::setw(8) << fastingStr
                          << "|"
                          << std::setw(10) << data
                          << "|"
                          << std::setw(5) << hora
                          << "\n";
                
                recordCount++;
            }
            
            std::cout << std::string(38, '-') << "\n";
            std::cout << "Total de registros: " << recordCount << "\n";
            std::cout << std::string(38, '-') << "\n";
            
            sqlite3_finalize(stmt);
        }
        else {
            std::cerr << "Erro ao preparar consulta: " << sqlite3_errmsg(db) << std::endl;
        }
        
        sqlite3_close(db);
    }
    catch(const std::exception& e) {
        std::cerr << "Exceção: " << e.what() << std::endl;
    }
}

void DatabaseMethods::displayDetailsMedicationRecordDB(int id){
        sqlite3* db;
    sqlite3_stmt* stmt;
    
    try {
        int rc = sqlite3_open("database.db", &db);
        if (rc != SQLITE_OK) {
            std::cerr << "Erro ao abrir database: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        const char* query = 
            "SELECT m.Nome, m.Horario, m.Dosagem, m.Medico , rs.Data, rs.Hora "
            "FROM RegistroMedicacao rm "
            "JOIN Medicacao m ON rm.Medicacao = m.Id "
            "JOIN RegistroSaude rs ON rm.RegistroSaude = rs.Id "
            "WHERE rs.Paciente = ? "
            "ORDER BY rs.Data DESC, rs.Hora DESC";
            
        if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, id);
            
            // Cabeçalho da tabela
            std::cout << "\n" << std::string(89, '-') << "\n";
            std::cout << "Registros de Medicacoes" << "\n";
            std::cout << std::string(89, '-') << "\n";
            
            std::cout << std::left
                      << std::setw(20) << "Medicamento"
                      << "|"
                      << std::setw(20) << "Horario"
                      << "|"
                      << std::setw(20) << "Medico"
                      << "|"
                      << std::setw(9) << "Dosagem"
                      << "|"
                      << std::setw(10) << "Data"
                      << "|"
                      << std::setw(5) << "Hora"
                      << "\n";
            std::cout << std::string(89, '-') << "\n";
    
            int recordCount = 0;
            
             while (sqlite3_step(stmt) == SQLITE_ROW) {
                std::string medication = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                std::string hours = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                std::string doctor = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                double dosage = sqlite3_column_double(stmt, 3);
                std::string date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
                std::string time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
                    
                // Exibir na tabela
                std::cout << std::left
                      << std::setw(20) << medication
                      << "|"
                      << std::setw(20) << hours
                      << "|"
                      << std::setw(20) << doctor
                      << "|"
                      << std::setw(7) << dosage << "mg"
                      << "|"
                      << std::setw(8) << date
                      << "|"
                      << std::setw(5) << time
                      << "\n";
                
                recordCount++;
            }
            
            std::cout << std::string(89, '-') << "\n";
            std::cout << "Total de registros: " << recordCount << "\n";
            std::cout << std::string(89, '-') << "\n";
            
            sqlite3_finalize(stmt);
        }
        else {
            std::cerr << "Erro ao preparar consulta: " << sqlite3_errmsg(db) << std::endl;
        }
        
        sqlite3_close(db);
    }
    catch(const std::exception& e) {
        std::cerr << "Exceção: " << e.what() << std::endl;
    }
}