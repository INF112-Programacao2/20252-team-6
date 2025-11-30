#include "../include/MedicationRecord.hpp"
#include "../include/HealthRecord.hpp"
#include "../include/Patient.hpp"
#include "../include/Medication.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <sqlite3.h>
#include <stdexcept>

// Construtor - inicializa HealthRecord e guarda referência pro medicamento
MedicationRecord::MedicationRecord(const Patient& patient, std::string date, Time hour, const Medication& medication)
    : HealthRecord(patient, date, hour), medication(medication)
{
}

MedicationRecord::~MedicationRecord()
{
}

// Salva o registro no banco em duas etapas:
// 1. Insere em RegistroSaude (tabela pai)
// 2. Insere em RegistroMedicacao (tabela filha) usando o ID gerado
void MedicationRecord::registerDB(int patientId)
{
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_stmt* stmt2 = nullptr;
    int registroSaudeId = -1;

    try {
        // Abre conexão com o banco
        int rc = sqlite3_open("database.db", &db);
        if (rc != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao abrir banco de dados: ") + sqlite3_errmsg(db));
        }

        // Primeiro passo: inserir na tabela pai (RegistroSaude)
        const char* sql1 = "INSERT INTO RegistroSaude (Paciente, Data, Hora) VALUES (?, ?, ?)";
        
        if (sqlite3_prepare_v2(db, sql1, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao preparar consulta (RegistroSaude): ") + sqlite3_errmsg(db));
        }

        // Converte Time para string no formato HH:MM:SS
        std::ostringstream timeStr;
        timeStr << std::setfill('0') << std::setw(2) << getHour().getHour() << ":"
                << std::setfill('0') << std::setw(2) << getHour().getMinute() << ":"
                << std::setfill('0') << std::setw(2) << getHour().getSecond();

        sqlite3_bind_int(stmt, 1, patientId);
        sqlite3_bind_text(stmt, 2, getDate().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, timeStr.str().c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            throw std::runtime_error(std::string("Erro ao executar inserção em RegistroSaude: ") + sqlite3_errmsg(db));
        }

        // Pega o ID gerado pra usar na tabela filha
        registroSaudeId = static_cast<int>(sqlite3_last_insert_rowid(db));
        sqlite3_finalize(stmt);
        stmt = nullptr;

        // Segundo passo: inserir na tabela filha (RegistroMedicacao)
        const char* sql2 = "INSERT INTO RegistroMedicacao (RegistroSaude, Medicacao) VALUES (?, ?)";
        
        if (sqlite3_prepare_v2(db, sql2, -1, &stmt2, nullptr) != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao preparar consulta (RegistroMedicacao): ") + sqlite3_errmsg(db));
        }

        sqlite3_bind_int(stmt2, 1, registroSaudeId);
        sqlite3_bind_int(stmt2, 2, medication.getId());

        if (sqlite3_step(stmt2) != SQLITE_DONE) {
            throw std::runtime_error(std::string("Erro ao executar inserção em RegistroMedicacao: ") + sqlite3_errmsg(db));
        }

        std::cout << "Registro de medicação salvo com sucesso!" << std::endl;
        
        sqlite3_finalize(stmt2);
        sqlite3_close(db);

    } catch (const std::exception& e) {
        // Tratamento de exceções - limpa todos os recursos
        std::cerr << "Exceção capturada em registerDB: " << e.what() << std::endl;
        if (stmt) {
            sqlite3_finalize(stmt);
        }
        if (stmt2) {
            sqlite3_finalize(stmt2);
        }
        if (db) {
            sqlite3_close(db);
        }
        throw;
    } catch (...) {
        // Catch genérico
        std::cerr << "Erro desconhecido ao registrar medicação." << std::endl;
        if (stmt) {
            sqlite3_finalize(stmt);
        }
        if (stmt2) {
            sqlite3_finalize(stmt2);
        }
        if (db) {
            sqlite3_close(db);
        }
        throw;
    }
}

// Mostra todos os detalhes do registro de medicação
void MedicationRecord::displayDetails()
{
    std::cout << "\n=== Registro de Medicação ===" << std::endl;
    std::cout << "Data: " << getDate() << std::endl;
    std::cout << "Hora: ";
    getHour().displayTime24();
    std::cout << "Medicamento: " << medication.getName() << std::endl;
    std::cout << "Princípio Ativo: " << medication.getActiveIngredient() << std::endl;
    std::cout << "Dosagem: " << medication.getDosage() << std::endl;
    std::cout << "Intervalo de administração: ";
    medication.getTimeMedication().displayTime24();
    std::cout << "Médico responsável: " << medication.getDoctor() << std::endl;
    std::cout << "Paciente: " << getPatient().getName() << std::endl;
}

// Getter pro medicamento
const Medication& MedicationRecord::getMedication() const
{
    return medication;
}

