#include "../include/Patient.hpp"
#include "../include/Person.hpp"
#include <iostream>
#include <sqlite3.h>
#include <algorithm>
#include <cctype>
#include <locale>
#include <string>
#include <stdexcept>

Patient::Patient(std::string name, std::string cpf, std::string adress,
                 std::string gender, int age, std::string password, std::string diabetesType, std::string bloodType,
                 double weight, double height)
    : Person(name, cpf, adress, gender, age, password), diabetesType(diabetesType), bloodType(bloodType),
      weight(weight), height(height) {}

std::string Patient::getDiabetesType() const
{
  return this->diabetesType;
}
std::string Patient::getBloodType() const
{
  return this->bloodType;
}
double Patient::getWeight() const
{
  return this->weight;
}
double Patient::getHeight() const
{
  return this->height;
}
bool Patient::login(std::string inputCpf, std::string inputPassword) {
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    bool loginSuccess = false;

    try {
        int rcOpen = sqlite3_open("database.db", &db);
        if (rcOpen != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao abrir banco de dados: ") + sqlite3_errmsg(db));
        }

        // Primeiro: verificar CPF e senha na tabela Pessoa
        const char* sqlPessoa = "SELECT id FROM Pessoa WHERE Cpf = ? AND Senha = ?";
        
        if (sqlite3_prepare_v2(db, sqlPessoa, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao preparar consulta (Pessoa): ") + sqlite3_errmsg(db));
        }

        sqlite3_bind_text(stmt, 1, inputCpf.c_str(), -1, SQLITE_STATIC);
        // Tratar Senha como TEXT (mesmo que o esquema diga INTEGER, SQLite aceita ambos)
        sqlite3_bind_text(stmt, 2, inputPassword.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int pessoaId = sqlite3_column_int(stmt, 0);
            sqlite3_finalize(stmt);
            stmt = nullptr;

            // Segundo: verificar se essa pessoa existe na tabela Paciente
            const char* sqlPaciente = "SELECT Id FROM Paciente WHERE Pessoa = ?";
            
            if (sqlite3_prepare_v2(db, sqlPaciente, -1, &stmt, nullptr) != SQLITE_OK) {
                throw std::runtime_error(std::string("Erro ao preparar consulta (Paciente): ") + sqlite3_errmsg(db));
            }

            sqlite3_bind_int(stmt, 1, pessoaId);
            
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                int pacienteId = sqlite3_column_int(stmt, 0);
                loginSuccess = true;
                std::cout << "Login realizado com sucesso! ID do Paciente: " << pacienteId << std::endl;
            } else {
                std::cout << "CPF ou senha incorretos, ou usuário não é um paciente." << std::endl;
            }
            sqlite3_finalize(stmt);
        } else {
            std::cout << "CPF ou senha incorretos." << std::endl;
            sqlite3_finalize(stmt);
        }

        sqlite3_close(db);
    } catch (const std::exception& e) {
        std::cerr << "Exceção capturada em login: " << e.what() << std::endl;
        if (stmt) {
            sqlite3_finalize(stmt);
        }
        if (db) {
            sqlite3_close(db);
        }
        return false;
    } catch (...) {
        std::cerr << "Erro desconhecido durante o login." << std::endl;
        if (stmt) {
            sqlite3_finalize(stmt);
        }
        if (db) {
            sqlite3_close(db);
        }
        return false;
    }

    return loginSuccess;
}

void Patient::makeAppointment(std::string date, std::string hour, std::string doctor, std::string specialty, std::string description, std::string location) {
    std::cout << "Agendando consulta..." << std::endl;
    std::cout << "Data: " << date << std::endl;
    std::cout << "Hora: " << hour << std::endl;
    std::cout << "Médico: " << doctor << std::endl;
    std::cout << "Especialidade: " << specialty << std::endl;
    std::cout << "Descrição: " << description << std::endl;
    std::cout << "Local: " << location << std::endl;
    // TODO: Implementar criação de ConsultationRecord e registro no banco
}

void Patient::cancelAppointment(int appointmentId) {
    std::cout << "Cancelando consulta com ID: " << appointmentId << std::endl;
    // TODO: Implementar cancelamento da consulta no banco de dados
}

void Patient::bookExam(std::string date, std::string hour, std::string nameExam, std::string doctor, std::string lab, std::string result) {
    std::cout << "Agendando exame..." << std::endl;
    std::cout << "Data: " << date << std::endl;
    std::cout << "Hora: " << hour << std::endl;
    std::cout << "Nome do exame: " << nameExam << std::endl;
    std::cout << "Médico: " << doctor << std::endl;
    std::cout << "Laboratório: " << lab << std::endl;
    std::cout << "Resultado: " << result << std::endl;
    // TODO: Implementar criação de ExamRecord e registro no banco
}

void Patient::cancelExam(int examId) {
    std::cout << "Cancelando exame com ID: " << examId << std::endl;
    // TODO: Implementar cancelamento do exame no banco de dados
}

void Patient::printClinicalState() const {
    std::cout << "Estado clínico do paciente:" << std::endl;
    std::cout << "Nome: " << this->getName() << std::endl;
    std::cout << "CPF: " << this->getCpf() << std::endl;
    std::cout << "Tipo de diabetes: " << this->diabetesType << std::endl;
    std::cout << "Tipo sanguíneo: " << this->bloodType << std::endl;
    std::cout << "Peso: " << this->weight << " kg" << std::endl;
    std::cout << "Altura: " << this->height << " m" << std::endl;
}

void Patient::registerClinicalData(std::string date, std::string hour, std::string description) {
    std::cout << "Registrando dados clínicos..." << std::endl;
    std::cout << "Data: " << date << std::endl;
    std::cout << "Hora: " << hour << std::endl;
    std::cout << "Descrição: " << description << std::endl;
    // TODO: Implementar registro de dados clínicos no banco de dados
}
void Patient::imprimirRegister() const {
    std::cout << "Imprimindo registro do paciente..." << std::endl;
    this->printClinicalState();
    // TODO: Implementar impressão de registros completos do paciente
}

//limpa as strings de possiveis espaços em branco
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

int Patient::searchId(){
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    int idPerson = -1;
    int idPatient = -1;

    try {
        int rc_open = sqlite3_open("database.db", &db);
        if (rc_open != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao abrir banco de dados: ") + sqlite3_errmsg(db));
        }
        
        // Buscar ID da Pessoa
        const char* sql = "SELECT id FROM Pessoa WHERE Nome = ? AND Cpf = ?"; 

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao preparar consulta (Pessoa): ") + sqlite3_errmsg(db));
        }

        // Limpar as strings antes de vincular
        std::string cleanName = trim(this->getName());
        std::string cleanCpf = trim(this->getCpf());
        
        sqlite3_bind_text(stmt, 1, cleanName.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, cleanCpf.c_str(), -1, SQLITE_STATIC);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            idPerson = sqlite3_column_int(stmt, 0);
            std::cout << "Pessoa ID: " << idPerson << std::endl;
        } else {
            std::cout << "Pessoa nao encontrada para Nome/Cpf: " << cleanName << " / " << cleanCpf << std::endl;
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;

        // Se a pessoa não foi encontrada, a busca do Paciente é impossível
        if (idPerson == -1) {
            sqlite3_close(db);
            return -1;
        }

        // Buscar ID do Paciente
        const char* sql2 = "SELECT Id FROM Paciente WHERE Pessoa = ?"; 

        if (sqlite3_prepare_v2(db, sql2, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao preparar consulta (Paciente): ") + sqlite3_errmsg(db));
        }

        sqlite3_bind_int(stmt, 1, idPerson);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            idPatient = sqlite3_column_int(stmt, 0);
            std::cout << "Paciente ID: " << idPatient << std::endl;
        } else {
            std::cout << "Paciente nao encontrado para Pessoa ID: " << idPerson << std::endl;
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);

    } catch (const std::exception& e) {
        std::cerr << "Exceção capturada em searchId: " << e.what() << std::endl;
        if (stmt) {
            sqlite3_finalize(stmt);
        }
        if (db) {
            sqlite3_close(db);
        }
        return -1;
    } catch (...) {
        std::cerr << "Erro desconhecido em searchId." << std::endl;
        if (stmt) {
            sqlite3_finalize(stmt);
        }
        if (db) {
            sqlite3_close(db);
        }
        return -1;
    }

    return idPatient;
}

bool Patient::verifyLogin(const std::string& inputCpf, const std::string& inputPassword) {
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    bool login_success = false;

    try {
        int rc_open = sqlite3_open("database.db", &db);
        if (rc_open != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao abrir banco de dados: ") + sqlite3_errmsg(db));
        }

        const char* sql = "SELECT Senha FROM Pessoa WHERE Cpf = ?";

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao preparar consulta (Pessoa): ") + sqlite3_errmsg(db));
        }

        sqlite3_bind_text(stmt, 1, inputCpf.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char* dbPassword = sqlite3_column_text(stmt, 0);
            if (dbPassword && inputPassword == reinterpret_cast<const char*>(dbPassword)) {
                login_success = true;
                std::cout << "Login bem-sucedido!" << std::endl;
            } else {
                std::cout << "CPF ou senha incorretos." << std::endl;
            }
        } else {
            std::cout << "CPF nao encontrado." << std::endl;
        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);

    } catch (const std::exception& e) {
        std::cerr << "Exceção capturada em verifyLogin: " << e.what() << std::endl;
        if (stmt) {
            sqlite3_finalize(stmt);
        }
        if (db) {
            sqlite3_close(db);
        }
        return false;
    } catch (...) {
        std::cerr << "Erro desconhecido durante verificação de login." << std::endl;
        if (stmt) {
            sqlite3_finalize(stmt);
        }
        if (db) {
            sqlite3_close(db);
        }
        return false;
    }

    return login_success;
}