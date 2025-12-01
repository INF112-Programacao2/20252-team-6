#include "../include/Medication.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <sqlite3.h>

// Construtor pra novo medicamento (id = -1, será gerado quando salvar no banco)
Medication::Medication(int patientId, std::string name, std::string activeIngredient, Time timeMedication,
                       double dosage, std::string doctor) 
    : id(-1), patientId(patientId), name(name), activeIngredient(activeIngredient),
      timeMedication(timeMedication), dosage(dosage), doctor(doctor)
{
    // Validações dos dados
    if (patientId <= 0) {
        throw std::invalid_argument("ID do paciente deve ser maior que zero.");
    }
    if (name.empty()) {
        throw std::invalid_argument("Nome do medicamento não pode ser vazio.");
    }
    if (activeIngredient.empty()) {
        throw std::invalid_argument("Princípio ativo não pode ser vazio.");
    }
    if (dosage <= 0) {
        throw std::invalid_argument("Dosagem deve ser maior que zero.");
    }
    if (dosage > 10000) {
        throw std::invalid_argument("Dosagem inválida. Máximo 10000 mg.");
    }
    if (doctor.empty()) {
        throw std::invalid_argument("Nome do médico não pode ser vazio.");
    }
}

// Construtor pra carregar medicamento do banco (já tem id)
Medication::Medication(int id, int patientId, std::string name, std::string activeIngredient, Time timeMedication,
                       double dosage, std::string doctor) 
    : id(id), patientId(patientId), name(name), activeIngredient(activeIngredient),
      timeMedication(timeMedication), dosage(dosage), doctor(doctor)
{
    // Validações (incluindo o id agora)
    if (id <= 0) {
        throw std::invalid_argument("ID do medicamento deve ser maior que zero.");
    }
    if (patientId <= 0) {
        throw std::invalid_argument("ID do paciente deve ser maior que zero.");
    }
    if (name.empty()) {
        throw std::invalid_argument("Nome do medicamento não pode ser vazio.");
    }
    if (activeIngredient.empty()) {
        throw std::invalid_argument("Princípio ativo não pode ser vazio.");
    }
    if (dosage <= 0) {
        throw std::invalid_argument("Dosagem deve ser maior que zero.");
    }
    if (dosage > 10000) {
        throw std::invalid_argument("Dosagem inválida. Máximo 10000 mg.");
    }
    if (doctor.empty()) {
        throw std::invalid_argument("Nome do médico não pode ser vazio.");
    }
}

Medication::~Medication(){}

// Printa todas as infos do medicamento
void Medication::printInfo() const
{
  std::cout << "Nome: " << this->name << "\n";
  std::cout << "Principio Ativo: " << this->activeIngredient << "\n";
  std::cout << "Intervalo: ";
  this->timeMedication.displayTime24();
  std::cout << "Dosagem: " << this->dosage << "\n";
  std::cout << "Medico: " << this->doctor << "\n";
}

// Getters simples
std::string Medication::getName() const
{
  return this->name;
}

std::string Medication::getActiveIngredient() const
{
  return this->activeIngredient;
}

double Medication::getDosage() const
{
  return this->dosage;
}

int Medication::getId() const
{
  return this->id;
}

int Medication::getPatientId() const
{
  return this->patientId;
}

std::string Medication::getDoctor() const
{
  return this->doctor;
}

Time Medication::getTimeMedication() const
{
  return this->timeMedication;
}

// Setter da dosagem - valida antes de setar
void Medication::setDosage(double dosage)
{
  if (dosage <= 0) {
    throw std::invalid_argument("Dosagem deve ser maior que zero.");
  }
  if (dosage > 10000) {
    throw std::invalid_argument("Dosagem inválida. Máximo 10000 mg.");
  }
  this->dosage = dosage;
}

// Salva o medicamento no banco de dados
void Medication::saveToDB()
{
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;

    try {
        // Abre conexão com o banco
        int rc = sqlite3_open("database.db", &db);
        if (rc != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao abrir banco de dados: ") + sqlite3_errmsg(db));
        }

        // Converte o horário (Time) pra string no formato HH:MM:SS
        std::ostringstream timeStr;
        timeStr << std::setfill('0') << std::setw(2) << timeMedication.getHour() << ":"
                << std::setfill('0') << std::setw(2) << timeMedication.getMinute() << ":"
                << std::setfill('0') << std::setw(2) << timeMedication.getSecond();
        
        // Verificar se a medicação já existe
        const char* sqlCheck = nullptr;
        if (this->id > 0) {
            // Medicação já foi salva antes - verifica por ID
            sqlCheck = "SELECT Id FROM Medicacao WHERE Id = ?";
        } else {
            // Medicação nova - verifica por Paciente + Nome + Horario
            sqlCheck = "SELECT Id FROM Medicacao WHERE Paciente = ? AND Nome = ? AND Horario = ?";
        }
        
        if (sqlite3_prepare_v2(db, sqlCheck, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao preparar consulta (verificar Medicacao): ") + sqlite3_errmsg(db));
        }

        if (this->id > 0) {
            sqlite3_bind_int(stmt, 1, this->id);
        } else {
            sqlite3_bind_int(stmt, 1, patientId);
            sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, timeStr.str().c_str(), -1, SQLITE_TRANSIENT);
        }
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Medicação já existe - pega o ID e atualiza
            int medicacaoId = sqlite3_column_int(stmt, 0);
            sqlite3_finalize(stmt);
            stmt = nullptr;

            // UPDATE na tabela Medicacao
            const char* sqlUpdate = "UPDATE Medicacao SET Dosagem = ?, Medico = ? WHERE Id = ?";
            
            if (sqlite3_prepare_v2(db, sqlUpdate, -1, &stmt, nullptr) != SQLITE_OK) {
                throw std::runtime_error(std::string("Erro ao preparar UPDATE (Medicacao): ") + sqlite3_errmsg(db));
            }

            sqlite3_bind_double(stmt, 1, dosage);
            sqlite3_bind_text(stmt, 2, doctor.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 3, medicacaoId);

            if (sqlite3_step(stmt) != SQLITE_DONE) {
                throw std::runtime_error(std::string("Erro ao executar UPDATE (Medicacao): ") + sqlite3_errmsg(db));
            }

            // Atualiza o ID do objeto se ainda não estava setado
            if (this->id <= 0) {
                this->id = medicacaoId;
            }

            std::cout << "Medicamento atualizado. ID: " << medicacaoId << std::endl;
            sqlite3_finalize(stmt);
            stmt = nullptr;
        } else {
            // Medicação não existe - insere nova
            sqlite3_finalize(stmt);
            stmt = nullptr;

            const char* sqlInsert = "INSERT INTO Medicacao (Paciente, Nome, Horario, Dosagem, Medico) VALUES (?, ?, ?, ?, ?)";
            
            if (sqlite3_prepare_v2(db, sqlInsert, -1, &stmt, nullptr) != SQLITE_OK) {
                throw std::runtime_error(std::string("Erro ao preparar INSERT (Medicacao): ") + sqlite3_errmsg(db));
            }

            sqlite3_bind_int(stmt, 1, patientId);
            sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, timeStr.str().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_double(stmt, 4, dosage);
            sqlite3_bind_text(stmt, 5, doctor.c_str(), -1, SQLITE_TRANSIENT);

            // Executa a inserção
            if (sqlite3_step(stmt) != SQLITE_DONE) {
                throw std::runtime_error(std::string("Erro ao executar INSERT (Medicacao): ") + sqlite3_errmsg(db));
            }

            // Pega o ID gerado pelo banco e atualiza o objeto
            this->id = static_cast<int>(sqlite3_last_insert_rowid(db));
            
            std::cout << "Novo medicamento inserido. ID: " << this->id << std::endl;
            sqlite3_finalize(stmt);
            stmt = nullptr;
        }

        sqlite3_close(db);
        std::cout << "Medicamento salvo no banco de dados com sucesso!" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Exceção capturada em saveToDB: " << e.what() << std::endl;
        if (stmt) {
            sqlite3_finalize(stmt);
        }
        if (db) {
            sqlite3_close(db);
        }
        throw;
    } catch (...) {
        std::cerr << "Erro desconhecido ao salvar medicamento." << std::endl;
        if (stmt) {
            sqlite3_finalize(stmt);
        }
        if (db) {
            sqlite3_close(db);
        }
        throw;
    }
}