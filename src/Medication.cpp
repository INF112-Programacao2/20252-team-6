#include "../include/Medication.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <sqlite3.h>

Medication::Medication(int patientId, std::string name, std::string activeIngredient, Time timeMedication,
                       double dosage, std::string doctor) 
    : id(-1), patientId(patientId), name(name), activeIngredient(activeIngredient),
      timeMedication(timeMedication), dosage(dosage), doctor(doctor)
{
}

Medication::Medication(int id, int patientId, std::string name, std::string activeIngredient, Time timeMedication,
                       double dosage, std::string doctor) 
    : id(id), patientId(patientId), name(name), activeIngredient(activeIngredient),
      timeMedication(timeMedication), dosage(dosage), doctor(doctor)
{
}

Medication::~Medication(){}

void Medication::printInfo() const
{
  std::cout << "Nome: " << this->name << "\n";
  std::cout << "Principio Ativo: " << this->activeIngredient << "\n";
  std::cout << "Horario: ";
  this->timeMedication.displayTime24();
  std::cout << "Dosagem: " << this->dosage << "\n";
  std::cout << "Medico: " << this->doctor << "\n";
}

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

void Medication::setDosage(double dosage)
{
  this->dosage = dosage;
}

void Medication::saveToDB()
{
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;

    try {
        int rc = sqlite3_open("database.db", &db);
        if (rc != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao abrir banco de dados: ") + sqlite3_errmsg(db));
        }

        std::ostringstream timeStr;
        timeStr << std::setfill('0') << std::setw(2) << timeMedication.getHour() << ":"
                << std::setfill('0') << std::setw(2) << timeMedication.getMinute() << ":"
                << std::setfill('0') << std::setw(2) << timeMedication.getSecond();
        
        const char* sql = "INSERT INTO Medicacao (Paciente, Nome, Horario, Dosagem, Medico) VALUES (?, ?, ?, ?, ?)";
        
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao preparar consulta: ") + sqlite3_errmsg(db));
        }

        sqlite3_bind_int(stmt, 1, patientId);
        sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, timeStr.str().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 4, dosage);
        sqlite3_bind_text(stmt, 5, doctor.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            throw std::runtime_error(std::string("Erro ao executar inserção: ") + sqlite3_errmsg(db));
        }

        // Obter o ID gerado
        this->id = static_cast<int>(sqlite3_last_insert_rowid(db));
        
        std::cout << "Medicamento registrado com sucesso! ID: " << this->id << std::endl;
        
        sqlite3_finalize(stmt);
        sqlite3_close(db);

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