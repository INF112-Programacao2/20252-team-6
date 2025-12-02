#include "../include/Medication.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <sqlite3.h>

// Construtor pra novo medicamento (id = -1, será gerado quando salvar no banco)
Medication::Medication(int patientId, std::string name, int timeMedication,
                       double dosage, std::string doctor) 
    : id(-1), patientId(patientId), name(name),
      timeMedication(timeMedication), dosage(dosage), doctor(doctor)
{
    if (patientId <= 0) {
        throw std::invalid_argument("ID do paciente deve ser maior que zero.");
    }
    if (name.empty()) {
        throw std::invalid_argument("Nome do medicamento não pode ser vazio.");
    }
    if (timeMedication <= 0) {
        throw std::invalid_argument("Intervalo deve ser maior que zero.");
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
Medication::Medication(int id, int patientId, std::string name, int timeMedication,
                       double dosage, std::string doctor) 
    : id(id), patientId(patientId), name(name),
      timeMedication(timeMedication), dosage(dosage), doctor(doctor)
{
    if (id <= 0) {
        throw std::invalid_argument("ID do medicamento deve ser maior que zero.");
    }
    if (patientId <= 0) {
        throw std::invalid_argument("ID do paciente deve ser maior que zero.");
    }
    if (name.empty()) {
        throw std::invalid_argument("Nome do medicamento não pode ser vazio.");
    }
    if (timeMedication <= 0) {
        throw std::invalid_argument("Intervalo deve ser maior que zero.");
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
  std::cout << "Intervalo: " << this->timeMedication << " horas\n";
  std::cout << "Dosagem: " << this->dosage << "\n";
  std::cout << "Medico: " << this->doctor << "\n";
}

// Getters simples
std::string Medication::getName() const
{
  return this->name;
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

int Medication::getTimeMedication() const
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
        int rc = sqlite3_open("database.db", &db);
        if (rc != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao abrir banco de dados: ") + sqlite3_errmsg(db));
        }

        std::string timeStr = std::to_string(timeMedication);
        
        const char* sqlCheck = nullptr;
        if (this->id > 0) {
            sqlCheck = "SELECT Id FROM Medicacao WHERE Id = ?";
        } else {
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
            sqlite3_bind_text(stmt, 3, timeStr.c_str(), -1, SQLITE_TRANSIENT);
        }
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int medicacaoId = sqlite3_column_int(stmt, 0);
            sqlite3_finalize(stmt);
            stmt = nullptr;

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

            if (this->id <= 0) {
                this->id = medicacaoId;
            }

            std::cout << "Medicamento atualizado. ID: " << medicacaoId << std::endl;
            sqlite3_finalize(stmt);
            stmt = nullptr;
        } else {
            sqlite3_finalize(stmt);
            stmt = nullptr;

            const char* sqlInsert = "INSERT INTO Medicacao (Paciente, Nome, Horario, Dosagem, Medico) VALUES (?, ?, ?, ?, ?)";
            
            if (sqlite3_prepare_v2(db, sqlInsert, -1, &stmt, nullptr) != SQLITE_OK) {
                throw std::runtime_error(std::string("Erro ao preparar INSERT (Medicacao): ") + sqlite3_errmsg(db));
            }

            sqlite3_bind_int(stmt, 1, patientId);
            sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, timeStr.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_double(stmt, 4, dosage);
            sqlite3_bind_text(stmt, 5, doctor.c_str(), -1, SQLITE_TRANSIENT);

            if (sqlite3_step(stmt) != SQLITE_DONE) {
                throw std::runtime_error(std::string("Erro ao executar INSERT (Medicacao): ") + sqlite3_errmsg(db));
            }

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

// Carrega medicamento do banco pelo ID
Medication* Medication::loadFromDB(int medicationID)
{
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    Medication* loadedMedication = nullptr;
    
    int id = -1, patientId = -1, timeMedication = 0;
    double dosage = 0.0;
    std::string name, doctor;

    try {
        int rc = sqlite3_open("database.db", &db);
        if (rc != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao abrir banco de dados: ") + sqlite3_errmsg(db));
        }

        const char* sql = "SELECT Id, Paciente, Nome, Horario, Dosagem, Medico FROM Medicacao WHERE Id = ?";
        
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao preparar consulta (Medicacao): ") + sqlite3_errmsg(db));
        }

        sqlite3_bind_int(stmt, 1, medicationID);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            auto get_text = [](sqlite3_stmt* s, int col) -> std::string {
                const unsigned char* text = sqlite3_column_text(s, col);
                return text ? reinterpret_cast<const char*>(text) : "";
            };

            id = sqlite3_column_int(stmt, 0);
            patientId = sqlite3_column_int(stmt, 1);
            name = get_text(stmt, 2);
            std::string timeStr = get_text(stmt, 3);
            timeMedication = std::stoi(timeStr);
            dosage = sqlite3_column_double(stmt, 4);
            doctor = get_text(stmt, 5);
            
            loadedMedication = new Medication(id, patientId, name, timeMedication, dosage, doctor);
            std::cout << "Medicamento carregado. ID: " << id << std::endl;
        } else {
            std::cout << "Medicamento com ID " << medicationID << " não encontrado." << std::endl;
            loadedMedication = nullptr;
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);

    } catch (const std::exception& e) {
        std::cerr << "Exceção ao carregar medicamento: " << e.what() << std::endl;
        if (stmt) sqlite3_finalize(stmt);
        if (db) sqlite3_close(db);
        return nullptr;
    } catch (...) {
        std::cerr << "Erro desconhecido ao carregar medicamento." << std::endl;
        if (stmt) sqlite3_finalize(stmt);
        if (db) sqlite3_close(db);
        return nullptr;
    }

    return loadedMedication;
}

// Carrega todos os medicamentos de um paciente
std::vector<Medication*> Medication::loadAllByPatient(int patientID)
{
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    std::vector<Medication*> medications;

    try {
        int rc = sqlite3_open("database.db", &db);
        if (rc != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao abrir banco de dados: ") + sqlite3_errmsg(db));
        }

        const char* sql = "SELECT Id, Paciente, Nome, Horario, Dosagem, Medico FROM Medicacao WHERE Paciente = ?";
        
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao preparar consulta (Medicacao): ") + sqlite3_errmsg(db));
        }

        sqlite3_bind_int(stmt, 1, patientID);

        auto get_text = [](sqlite3_stmt* s, int col) -> std::string {
            const unsigned char* text = sqlite3_column_text(s, col);
            return text ? reinterpret_cast<const char*>(text) : "";
        };

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            int patientId = sqlite3_column_int(stmt, 1);
            std::string name = get_text(stmt, 2);
            std::string timeStr = get_text(stmt, 3);
            int timeMedication = std::stoi(timeStr);
            double dosage = sqlite3_column_double(stmt, 4);
            std::string doctor = get_text(stmt, 5);
            
            Medication* med = new Medication(id, patientId, name, timeMedication, dosage, doctor);
            medications.push_back(med);
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::cout << "Carregados " << medications.size() << " medicamento(s) do paciente ID " << patientID << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Exceção ao carregar medicamentos: " << e.what() << std::endl;
        if (stmt) sqlite3_finalize(stmt);
        if (db) sqlite3_close(db);
        return medications;
    } catch (...) {
        std::cerr << "Erro desconhecido ao carregar medicamentos." << std::endl;
        if (stmt) sqlite3_finalize(stmt);
        if (db) sqlite3_close(db);
        return medications;
    }

    return medications;
}