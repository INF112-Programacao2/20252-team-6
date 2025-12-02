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
    // Valida se medicamento foi salvo no banco (tem ID válido)
    if (medication.getId() <= 0) {
        throw std::runtime_error("Medicamento deve ser salvo no banco antes de criar registro. Execute medication.saveToDB() primeiro.");
    }
    
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
    std::cout << "Dosagem: " << medication.getDosage() << std::endl;
    std::cout << "Intervalo de administração: " << medication.getTimeMedication() << " horas" << std::endl;
    std::cout << "Médico responsável: " << medication.getDoctor() << std::endl;
    std::cout << "Paciente: " << getPatient().getName() << std::endl;
}

// Getter pro medicamento
const Medication& MedicationRecord::getMedication() const
{
    return medication;
}

// Carrega registro do banco pelo ID
MedicationRecord* MedicationRecord::loadFromDB(int recordID)
{
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    MedicationRecord* loadedRecord = nullptr;
    
    int registroSaudeId = -1;
    int patientId = -1;
    int medicationId = -1;
    std::string date, hourStr;
    
    Patient* patient = nullptr;
    Medication* medication = nullptr;

    try {
        int rc = sqlite3_open("database.db", &db);
        if (rc != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao abrir banco de dados: ") + sqlite3_errmsg(db));
        }

        const char* sql = "SELECT RegistroSaude, Medicacao FROM RegistroMedicacao WHERE Id = ?";
        
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao preparar consulta (RegistroMedicacao): ") + sqlite3_errmsg(db));
        }

        sqlite3_bind_int(stmt, 1, recordID);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            registroSaudeId = sqlite3_column_int(stmt, 0);
            medicationId = sqlite3_column_int(stmt, 1);
        } else {
            std::cout << "Registro de medicação com ID " << recordID << " não encontrado." << std::endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return nullptr;
        }

        sqlite3_finalize(stmt);
        stmt = nullptr;

        const char* sql2 = "SELECT Paciente, Data, Hora FROM RegistroSaude WHERE Id = ?";
        
        if (sqlite3_prepare_v2(db, sql2, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao preparar consulta (RegistroSaude): ") + sqlite3_errmsg(db));
        }

        sqlite3_bind_int(stmt, 1, registroSaudeId);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            auto get_text = [](sqlite3_stmt* s, int col) -> std::string {
                const unsigned char* text = sqlite3_column_text(s, col);
                return text ? reinterpret_cast<const char*>(text) : "";
            };

            patientId = sqlite3_column_int(stmt, 0);
            date = get_text(stmt, 1);
            hourStr = get_text(stmt, 2);
        } else {
            std::cout << "RegistroSaude com ID " << registroSaudeId << " não encontrado." << std::endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return nullptr;
        }

        sqlite3_finalize(stmt);
        stmt = nullptr;

        medication = Medication::loadFromDB(medicationId);
        if (!medication) {
            std::cout << "Medicamento com ID " << medicationId << " não encontrado." << std::endl;
            sqlite3_close(db);
            return nullptr;
        }

        const char* sql3 = "SELECT p.Nome, p.Cpf, p.Endereco, p.Sexo, p.Idade, p.Senha, "
                          "pac.TipoDiabetes, pac.TipoSanguineo, pac.Peso, pac.Altura "
                          "FROM Pessoa p "
                          "INNER JOIN Paciente pac ON pac.Pessoa = p.id "
                          "WHERE pac.Id = ?";
        
        if (sqlite3_prepare_v2(db, sql3, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao preparar consulta (Paciente): ") + sqlite3_errmsg(db));
        }

        sqlite3_bind_int(stmt, 1, patientId);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            auto get_text = [](sqlite3_stmt* s, int col) -> std::string {
                const unsigned char* text = sqlite3_column_text(s, col);
                return text ? reinterpret_cast<const char*>(text) : "";
            };

            std::string name = get_text(stmt, 0);
            std::string cpf = get_text(stmt, 1);
            std::string adress = get_text(stmt, 2);
            std::string gender = get_text(stmt, 3);
            int age = sqlite3_column_int(stmt, 4);
            std::string password = get_text(stmt, 5);
            std::string diabetesType = get_text(stmt, 6);
            std::string bloodType = get_text(stmt, 7);
            double weight = sqlite3_column_double(stmt, 8);
            double height = sqlite3_column_double(stmt, 9);
            
            patient = new Patient(name, cpf, adress, gender, age, password, 
                                diabetesType, bloodType, weight, height);
        } else {
            std::cout << "Paciente com ID " << patientId << " não encontrado." << std::endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            delete medication;
            return nullptr;
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);

        Time hour(hourStr);
        loadedRecord = new MedicationRecord(*patient, date, hour, *medication);
        std::cout << "Registro de medicação carregado. ID: " << recordID << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Exceção ao carregar registro: " << e.what() << std::endl;
        if (stmt) sqlite3_finalize(stmt);
        if (db) sqlite3_close(db);
        if (medication) delete medication;
        if (patient) delete patient;
        return nullptr;
    } catch (...) {
        std::cerr << "Erro desconhecido ao carregar registro." << std::endl;
        if (stmt) sqlite3_finalize(stmt);
        if (db) sqlite3_close(db);
        if (medication) delete medication;
        if (patient) delete patient;
        return nullptr;
    }

    return loadedRecord;
}

