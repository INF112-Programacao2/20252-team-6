#include "../include/Patient.hpp"
#include "../include/Person.hpp"
#include "../include/ConsultationRecord.hpp"
#include "../include/ExamRecord.hpp"
#include "../include/Time.hpp"
#include <iostream>
#include <sqlite3.h>
#include <algorithm>
#include <cctype>
#include <locale>
#include <string>
#include <stdexcept>

// Construtor - chama construtor de Person e adiciona validações médicas
Patient::Patient(std::string name, std::string cpf, std::string adress,
                 std::string gender, int age, std::string password, std::string diabetesType, std::string bloodType,
                 double weight, double height)
    : Person(name, cpf, adress, gender, age, password), diabetesType(diabetesType), bloodType(bloodType),
      weight(weight), height(height) 
{
    // Validações dos dados médicos
    if (weight <= 0) {
        throw std::invalid_argument("Peso deve ser maior que zero.");
    }
    if (height <= 0) {
        throw std::invalid_argument("Altura deve ser maior que zero.");
    }
    if (diabetesType.empty()) {
        throw std::invalid_argument("Tipo de diabetes não pode ser vazio.");
    }
    if (bloodType.empty()) {
        throw std::invalid_argument("Tipo sanguíneo não pode ser vazio.");
    }
}

// Getters simples dos dados médicos
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

// Login - verifica CPF e senha no banco de dados
// Retorna true se login ok, false caso contrário
bool Patient::login(std::string inputCpf, std::string inputPassword) {
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    bool loginSuccess = false;

    try {
        // Abre conexão com o banco
        int rcOpen = sqlite3_open("database.db", &db);
        if (rcOpen != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao abrir banco de dados: ") + sqlite3_errmsg(db));
        }

        // Primeiro passo: verifica CPF e senha na tabela Pessoa
        const char* sqlPessoa = "SELECT id FROM Pessoa WHERE Cpf = ? AND Senha = ?";
        
        if (sqlite3_prepare_v2(db, sqlPessoa, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao preparar consulta (Pessoa): ") + sqlite3_errmsg(db));
        }

        // Bind dos parâmetros (prepared statement pra evitar SQL injection)
        sqlite3_bind_text(stmt, 1, inputCpf.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, inputPassword.c_str(), -1, SQLITE_STATIC);

        // Executa a query
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int pessoaId = sqlite3_column_int(stmt, 0);
            sqlite3_finalize(stmt);
            stmt = nullptr;

            // Segundo passo: verifica se essa pessoa é um paciente
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
        // Tratamento de exceções - sempre limpa recursos
        std::cerr << "Exceção capturada em login: " << e.what() << std::endl;
        if (stmt) {
            sqlite3_finalize(stmt);
        }
        if (db) {
            sqlite3_close(db);
        }
        return false;
    } catch (...) {
        // Catch genérico pra qualquer erro não esperado
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

// Agenda uma consulta médica
// Valida os dados antes de agendar
void Patient::makeAppointment(std::string date, std::string hour, std::string doctor, std::string specialty, std::string description, std::string location) {
    // Validações básicas
    if (date.empty()) {
        throw std::invalid_argument("Data não pode ser vazia.");
    }
    if (hour.empty()) {
        throw std::invalid_argument("Hora não pode ser vazia.");
    }
    if (doctor.empty()) {
        throw std::invalid_argument("Nome do médico não pode ser vazio.");
    }
    if (specialty.empty()) {
        throw std::invalid_argument("Especialidade não pode ser vazia.");
    }
    if (location.empty()) {
        throw std::invalid_argument("Local não pode ser vazio.");
    }
    
    // Busca ID do paciente no banco
    int patientId = this->searchId();
    if (patientId == -1) {
        throw std::runtime_error("Paciente não encontrado no banco. Execute saveToDB() primeiro.");
    }
    
    // Cria e salva consulta no banco
    try {
        Time timeObj(hour);  // Construtor de Time a partir de string
        ConsultationRecord consulta(*this, date, timeObj, doctor, specialty, description, location);
        consulta.registerDB(patientId);
        std::cout << "Consulta agendada com sucesso!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Erro ao agendar consulta: " << e.what() << std::endl;
        throw;
    }
}

// Cancela uma consulta agendada
void Patient::cancelAppointment(int appointmentId) {
    if (appointmentId <= 0) {
        throw std::invalid_argument("ID da consulta deve ser maior que zero.");
    }
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    
    try {
        int rc = sqlite3_open("database.db", &db);
        if (rc != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao abrir banco: ") + sqlite3_errmsg(db));
        }
        
        // Busca RegistroSaude associado
        const char* sqlFind = "SELECT RegistroSaude FROM RegistroConsulta WHERE Id = ?";
        if (sqlite3_prepare_v2(db, sqlFind, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao preparar consulta: ") + sqlite3_errmsg(db));
        }
        
        sqlite3_bind_int(stmt, 1, appointmentId);
        int registroSaudeId = -1;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            registroSaudeId = sqlite3_column_int(stmt, 0);
        } else {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            throw std::runtime_error("Consulta não encontrada.");
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
        
        // Deleta consulta
        const char* sqlDelete = "DELETE FROM RegistroConsulta WHERE Id = ?";
        if (sqlite3_prepare_v2(db, sqlDelete, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao preparar DELETE: ") + sqlite3_errmsg(db));
        }
        sqlite3_bind_int(stmt, 1, appointmentId);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            throw std::runtime_error(std::string("Erro ao deletar consulta: ") + sqlite3_errmsg(db));
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
        
        // Verifica se há outros registros usando RegistroSaude
        const char* sqlCheck = "SELECT COUNT(*) FROM (SELECT 1 FROM RegistroExame WHERE RegistroSaude = ? UNION ALL SELECT 1 FROM RegistroMedicacao WHERE RegistroSaude = ? UNION ALL SELECT 1 FROM RegistroGlicose WHERE RegistroSaude = ?)";
        if (sqlite3_prepare_v2(db, sqlCheck, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao verificar: ") + sqlite3_errmsg(db));
        }
        sqlite3_bind_int(stmt, 1, registroSaudeId);
        sqlite3_bind_int(stmt, 2, registroSaudeId);
        sqlite3_bind_int(stmt, 3, registroSaudeId);
        int count = 0;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
        
        // Deleta RegistroSaude se não houver outros registros
        if (count == 0) {
            const char* sqlDeleteSaude = "DELETE FROM RegistroSaude WHERE Id = ?";
            if (sqlite3_prepare_v2(db, sqlDeleteSaude, -1, &stmt, nullptr) != SQLITE_OK) {
                throw std::runtime_error(std::string("Erro ao preparar DELETE RegistroSaude: ") + sqlite3_errmsg(db));
            }
            sqlite3_bind_int(stmt, 1, registroSaudeId);
            if (sqlite3_step(stmt) != SQLITE_DONE) {
                throw std::runtime_error(std::string("Erro ao deletar RegistroSaude: ") + sqlite3_errmsg(db));
            }
            sqlite3_finalize(stmt);
        }
        
        sqlite3_close(db);
        std::cout << "Consulta cancelada com sucesso!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Exceção em cancelAppointment: " << e.what() << std::endl;
        if (stmt) sqlite3_finalize(stmt);
        if (db) sqlite3_close(db);
        throw;
    } catch (...) {
        std::cerr << "Erro desconhecido ao cancelar consulta." << std::endl;
        if (stmt) sqlite3_finalize(stmt);
        if (db) sqlite3_close(db);
        throw;
    }
}

// Agenda um exame médico
void Patient::bookExam(std::string date, std::string hour, std::string nameExam, std::string doctor, std::string lab, std::string result) {
    if (date.empty()) {
        throw std::invalid_argument("Data não pode ser vazia.");
    }
    if (hour.empty()) {
        throw std::invalid_argument("Hora não pode ser vazia.");
    }
    if (nameExam.empty()) {
        throw std::invalid_argument("Nome do exame não pode ser vazio.");
    }
    if (doctor.empty()) {
        throw std::invalid_argument("Nome do médico não pode ser vazio.");
    }
    if (lab.empty()) {
        throw std::invalid_argument("Nome do laboratório não pode ser vazio.");
    }
    
    // Busca ID do paciente no banco
    int patientId = this->searchId();
    if (patientId == -1) {
        throw std::runtime_error("Paciente não encontrado no banco. Execute saveToDB() primeiro.");
    }
    
    // Cria e salva exame no banco
    try {
        Time timeObj(hour);  // Construtor de Time a partir de string
        ExamRecord exame(*this, date, timeObj, nameExam, result, lab, doctor);
        exame.registerDB(patientId);
        std::cout << "Exame agendado com sucesso!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Erro ao agendar exame: " << e.what() << std::endl;
        throw;
    }
}

// Cancela um exame agendado
void Patient::cancelExam(int examId) {
    if (examId <= 0) {
        throw std::invalid_argument("ID do exame deve ser maior que zero.");
    }
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    
    try {
        int rc = sqlite3_open("database.db", &db);
        if (rc != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao abrir banco: ") + sqlite3_errmsg(db));
        }
        
        // Busca RegistroSaude associado
        const char* sqlFind = "SELECT RegistroSaude FROM RegistroExame WHERE Id = ?";
        if (sqlite3_prepare_v2(db, sqlFind, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao preparar consulta: ") + sqlite3_errmsg(db));
        }
        
        sqlite3_bind_int(stmt, 1, examId);
        int registroSaudeId = -1;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            registroSaudeId = sqlite3_column_int(stmt, 0);
        } else {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            throw std::runtime_error("Exame não encontrado.");
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
        
        // Deleta exame
        const char* sqlDelete = "DELETE FROM RegistroExame WHERE Id = ?";
        if (sqlite3_prepare_v2(db, sqlDelete, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao preparar DELETE: ") + sqlite3_errmsg(db));
        }
        sqlite3_bind_int(stmt, 1, examId);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            throw std::runtime_error(std::string("Erro ao deletar exame: ") + sqlite3_errmsg(db));
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
        
        // Verifica se há outros registros usando RegistroSaude
        const char* sqlCheck = "SELECT COUNT(*) FROM (SELECT 1 FROM RegistroConsulta WHERE RegistroSaude = ? UNION ALL SELECT 1 FROM RegistroMedicacao WHERE RegistroSaude = ? UNION ALL SELECT 1 FROM RegistroGlicose WHERE RegistroSaude = ?)";
        if (sqlite3_prepare_v2(db, sqlCheck, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao verificar: ") + sqlite3_errmsg(db));
        }
        sqlite3_bind_int(stmt, 1, registroSaudeId);
        sqlite3_bind_int(stmt, 2, registroSaudeId);
        sqlite3_bind_int(stmt, 3, registroSaudeId);
        int count = 0;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
        
        // Deleta RegistroSaude se não houver outros registros
        if (count == 0) {
            const char* sqlDeleteSaude = "DELETE FROM RegistroSaude WHERE Id = ?";
            if (sqlite3_prepare_v2(db, sqlDeleteSaude, -1, &stmt, nullptr) != SQLITE_OK) {
                throw std::runtime_error(std::string("Erro ao preparar DELETE RegistroSaude: ") + sqlite3_errmsg(db));
            }
            sqlite3_bind_int(stmt, 1, registroSaudeId);
            if (sqlite3_step(stmt) != SQLITE_DONE) {
                throw std::runtime_error(std::string("Erro ao deletar RegistroSaude: ") + sqlite3_errmsg(db));
            }
            sqlite3_finalize(stmt);
        }
        
        sqlite3_close(db);
        std::cout << "Exame cancelado com sucesso!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Exceção em cancelExam: " << e.what() << std::endl;
        if (stmt) sqlite3_finalize(stmt);
        if (db) sqlite3_close(db);
        throw;
    } catch (...) {
        std::cerr << "Erro desconhecido ao cancelar exame." << std::endl;
        if (stmt) sqlite3_finalize(stmt);
        if (db) sqlite3_close(db);
        throw;
    }
}

// Mostra o estado clínico atual do paciente
void Patient::printClinicalState() const {
    std::cout << "Estado clínico do paciente:" << std::endl;
    std::cout << "Nome: " << this->getName() << std::endl;
    std::cout << "CPF: " << this->getCpf() << std::endl;
    std::cout << "Tipo de diabetes: " << this->diabetesType << std::endl;
    std::cout << "Tipo sanguíneo: " << this->bloodType << std::endl;
    std::cout << "Peso: " << this->weight << " kg" << std::endl;
    std::cout << "Altura: " << this->height << " m" << std::endl;
}

// Registra dados clínicos do paciente (peso, altura, sintomas, etc)
void Patient::registerClinicalData(std::string date, std::string hour, std::string description) {
    if (date.empty()) {
        throw std::invalid_argument("Data não pode ser vazia.");
    }
    if (hour.empty()) {
        throw std::invalid_argument("Hora não pode ser vazia.");
    }
    if (description.empty()) {
        throw std::invalid_argument("Descrição não pode ser vazia.");
    }
    
    // Busca ID do paciente no banco
    int patientId = this->searchId();
    if (patientId == -1) {
        throw std::runtime_error("Paciente não encontrado no banco. Execute saveToDB() primeiro.");
    }
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    
    try {
        int rc = sqlite3_open("database.db", &db);
        if (rc != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao abrir banco: ") + sqlite3_errmsg(db));
        }
        
        // Insere registro genérico de saúde
        const char* sql = "INSERT INTO RegistroSaude (Paciente, Data, Hora) VALUES (?, ?, ?)";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao preparar INSERT: ") + sqlite3_errmsg(db));
        }
        
        sqlite3_bind_int(stmt, 1, patientId);
        sqlite3_bind_text(stmt, 2, date.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, hour.c_str(), -1, SQLITE_TRANSIENT);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            throw std::runtime_error(std::string("Erro ao executar inserção: ") + sqlite3_errmsg(db));
        }
        
        int registroId = static_cast<int>(sqlite3_last_insert_rowid(db));
        std::cout << "Dados clínicos registrados. ID: " << registroId << std::endl;
        std::cout << "Descrição: " << description << std::endl;
        
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        
    } catch (const std::exception& e) {
        std::cerr << "Exceção em registerClinicalData: " << e.what() << std::endl;
        if (stmt) sqlite3_finalize(stmt);
        if (db) sqlite3_close(db);
        throw;
    } catch (...) {
        std::cerr << "Erro desconhecido ao registrar dados clínicos." << std::endl;
        if (stmt) sqlite3_finalize(stmt);
        if (db) sqlite3_close(db);
        throw;
    }
}

// Imprime todos os registros do paciente
void Patient::imprimirRegister() const {
    std::cout << "\n========================================" << std::endl;
    std::cout << "    REGISTRO COMPLETO DO PACIENTE" << std::endl;
    std::cout << "========================================" << std::endl;
    
    this->printClinicalState();
    
    // Busca ID do paciente
    int patientId = -1;
    sqlite3* dbTemp = nullptr;
    sqlite3_stmt* stmtTemp = nullptr;
    
    try {
        int rc = sqlite3_open("database.db", &dbTemp);
        if (rc == SQLITE_OK) {
            const char* sql = "SELECT p.Id FROM Paciente p JOIN Pessoa pe ON p.Pessoa = pe.id WHERE pe.Nome = ? AND pe.Cpf = ?";
            if (sqlite3_prepare_v2(dbTemp, sql, -1, &stmtTemp, nullptr) == SQLITE_OK) {
                sqlite3_bind_text(stmtTemp, 1, this->getName().c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_text(stmtTemp, 2, this->getCpf().c_str(), -1, SQLITE_STATIC);
                if (sqlite3_step(stmtTemp) == SQLITE_ROW) {
                    patientId = sqlite3_column_int(stmtTemp, 0);
                }
                sqlite3_finalize(stmtTemp);
            }
            sqlite3_close(dbTemp);
        }
    } catch (...) {
        if (stmtTemp) sqlite3_finalize(stmtTemp);
        if (dbTemp) sqlite3_close(dbTemp);
    }
    
    if (patientId == -1) {
        std::cout << "\nAviso: Paciente não encontrado no banco." << std::endl;
        return;
    }
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    
    try {
        int rc = sqlite3_open("database.db", &db);
        if (rc != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao abrir banco: ") + sqlite3_errmsg(db));
        }
        
        // Lista consultas
        std::cout << "\n--- CONSULTAS ---" << std::endl;
        const char* sqlConsultas = "SELECT rc.Medico, rc.Especialidade, rc.Descricao, rc.Local, rs.Data, rs.Hora FROM RegistroConsulta rc JOIN RegistroSaude rs ON rc.RegistroSaude = rs.Id WHERE rs.Paciente = ? ORDER BY rs.Data DESC, rs.Hora DESC";
        if (sqlite3_prepare_v2(db, sqlConsultas, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, patientId);
            int count = 0;
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                std::string medico = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                std::string especialidade = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                std::string descricao = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                std::string local = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                std::string data = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
                std::string hora = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
                std::cout << "\nConsulta #" << (++count) << std::endl;
                std::cout << "  Data/Hora: " << data << " " << hora << std::endl;
                std::cout << "  Médico: " << medico << " (" << especialidade << ")" << std::endl;
                std::cout << "  Local: " << local << std::endl;
                std::cout << "  Descrição: " << descricao << std::endl;
            }
            if (count == 0) std::cout << "Nenhuma consulta registrada." << std::endl;
            sqlite3_finalize(stmt);
        }
        
        // Lista exames
        std::cout << "\n--- EXAMES ---" << std::endl;
        const char* sqlExames = "SELECT re.Nome, re.Medico, re.Laboratorio, re.Resultado, rs.Data, rs.Hora FROM RegistroExame re JOIN RegistroSaude rs ON re.RegistroSaude = rs.Id WHERE rs.Paciente = ? ORDER BY rs.Data DESC, rs.Hora DESC";
        if (sqlite3_prepare_v2(db, sqlExames, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, patientId);
            int count = 0;
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                std::string nome = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                std::string medico = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                std::string lab = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                std::string resultado = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                std::string data = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
                std::string hora = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
                std::cout << "\nExame #" << (++count) << std::endl;
                std::cout << "  Data/Hora: " << data << " " << hora << std::endl;
                std::cout << "  Nome: " << nome << std::endl;
                std::cout << "  Médico: " << medico << std::endl;
                std::cout << "  Laboratório: " << lab << std::endl;
                std::cout << "  Resultado: " << resultado << std::endl;
            }
            if (count == 0) std::cout << "Nenhum exame registrado." << std::endl;
            sqlite3_finalize(stmt);
        }
        
        // Lista medicações
        std::cout << "\n--- USO DE MEDICAMENTOS ---" << std::endl;
        const char* sqlMedicacoes = "SELECT m.Nome, m.Dosagem, m.Horario, m.Medico, rs.Data, rs.Hora FROM RegistroMedicacao rm JOIN Medicacao m ON rm.Medicacao = m.Id JOIN RegistroSaude rs ON rm.RegistroSaude = rs.Id WHERE rs.Paciente = ? ORDER BY rs.Data DESC, rs.Hora DESC";
        if (sqlite3_prepare_v2(db, sqlMedicacoes, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, patientId);
            int count = 0;
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                std::string nome = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                double dosagem = sqlite3_column_double(stmt, 1);
                std::string horario = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                std::string medico = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                std::string data = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
                std::string hora = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
                std::cout << "\nMedicamento #" << (++count) << std::endl;
                std::cout << "  Data/Hora do uso: " << data << " " << hora << std::endl;
                std::cout << "  Medicamento: " << nome << std::endl;
                std::cout << "  Dosagem: " << dosagem << std::endl;
                std::cout << "  Horário programado: " << horario << std::endl;
                std::cout << "  Médico: " << medico << std::endl;
            }
            if (count == 0) std::cout << "Nenhum uso de medicamento registrado." << std::endl;
            sqlite3_finalize(stmt);
        }
        
        sqlite3_close(db);
        std::cout << "\n========================================" << std::endl;
        std::cout << "    FIM DO REGISTRO" << std::endl;
        std::cout << "========================================" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Exceção em imprimirRegister: " << e.what() << std::endl;
        if (stmt) sqlite3_finalize(stmt);
        if (db) sqlite3_close(db);
    } catch (...) {
        std::cerr << "Erro desconhecido ao imprimir registros." << std::endl;
        if (stmt) sqlite3_finalize(stmt);
        if (db) sqlite3_close(db);
    }
}

// Função auxiliar: limpa espaços em branco no início e fim da string
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

// Busca o ID do paciente no banco usando nome e CPF
// Retorna -1 se não encontrar
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
        
        // Primeiro: busca o ID da Pessoa
        const char* sql = "SELECT id FROM Pessoa WHERE Nome = ? AND Cpf = ?"; 

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao preparar consulta (Pessoa): ") + sqlite3_errmsg(db));
        }

        // Limpa espaços antes de buscar (evita problemas de comparação)
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

        // Se não encontrou a pessoa, não tem como encontrar o paciente
        if (idPerson == -1) {
            sqlite3_close(db);
            return -1;
        }

        // Segundo: busca o ID do Paciente usando o ID da Pessoa
        // Segundo: busca o ID do Paciente usando o ID da Pessoa
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

// Método estático pra verificar login sem precisar criar objeto Patient
// Útil quando só quer verificar credenciais
bool Patient::verifyLogin(const std::string& inputCpf, const std::string& inputPassword) {
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    bool login_success = false;

    try {
        int rc_open = sqlite3_open("database.db", &db);
        if (rc_open != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao abrir banco de dados: ") + sqlite3_errmsg(db));
        }

        // Busca a senha no banco usando o CPF
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

// Salva o paciente no banco de dados
// Primeiro insere/atualiza na tabela Pessoa, depois na tabela Paciente
// Se a pessoa já existe (pelo CPF), atualiza. Se não, insere nova.
void Patient::saveToDB()
{
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    int pessoaId = -1;

    try {
        // Abre conexão com o banco
        int rc = sqlite3_open("database.db", &db);
        if (rc != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao abrir banco de dados: ") + sqlite3_errmsg(db));
        }

        // Primeiro passo: verificar se a pessoa já existe (pelo CPF)
        const char* sqlCheck = "SELECT id FROM Pessoa WHERE Cpf = ?";
        
        if (sqlite3_prepare_v2(db, sqlCheck, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao preparar consulta (verificar Pessoa): ") + sqlite3_errmsg(db));
        }

        sqlite3_bind_text(stmt, 1, this->getCpf().c_str(), -1, SQLITE_STATIC);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Pessoa já existe - pega o ID e atualiza
            pessoaId = sqlite3_column_int(stmt, 0);
            sqlite3_finalize(stmt);
            stmt = nullptr;

            // UPDATE na tabela Pessoa
            const char* sqlUpdate = "UPDATE Pessoa SET Nome = ?, Sexo = ?, Senha = ?, Endereco = ? WHERE id = ?";
            
            if (sqlite3_prepare_v2(db, sqlUpdate, -1, &stmt, nullptr) != SQLITE_OK) {
                throw std::runtime_error(std::string("Erro ao preparar UPDATE (Pessoa): ") + sqlite3_errmsg(db));
            }

            sqlite3_bind_text(stmt, 1, this->getName().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, this->getGender().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, this->getPassword().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 4, this->getAdress().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 5, pessoaId);

            if (sqlite3_step(stmt) != SQLITE_DONE) {
                throw std::runtime_error(std::string("Erro ao executar UPDATE (Pessoa): ") + sqlite3_errmsg(db));
            }

            std::cout << "Dados da pessoa atualizados. ID: " << pessoaId << std::endl;
            sqlite3_finalize(stmt);
            stmt = nullptr;
        } else {
            // Pessoa não existe - insere nova
            sqlite3_finalize(stmt);
            stmt = nullptr;

            const char* sqlInsert = "INSERT INTO Pessoa (Nome, Cpf, Sexo, Senha, Endereco) VALUES (?, ?, ?, ?, ?)";
            
            if (sqlite3_prepare_v2(db, sqlInsert, -1, &stmt, nullptr) != SQLITE_OK) {
                throw std::runtime_error(std::string("Erro ao preparar INSERT (Pessoa): ") + sqlite3_errmsg(db));
            }

            sqlite3_bind_text(stmt, 1, this->getName().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, this->getCpf().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, this->getGender().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 4, this->getPassword().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 5, this->getAdress().c_str(), -1, SQLITE_TRANSIENT);

            if (sqlite3_step(stmt) != SQLITE_DONE) {
                throw std::runtime_error(std::string("Erro ao executar INSERT (Pessoa): ") + sqlite3_errmsg(db));
            }

            pessoaId = static_cast<int>(sqlite3_last_insert_rowid(db));
            std::cout << "Nova pessoa inserida. ID: " << pessoaId << std::endl;
            sqlite3_finalize(stmt);
            stmt = nullptr;
        }

        // Segundo passo: verificar se já existe registro na tabela Paciente
        const char* sqlCheckPaciente = "SELECT Id FROM Paciente WHERE Pessoa = ?";
        
        if (sqlite3_prepare_v2(db, sqlCheckPaciente, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error(std::string("Erro ao preparar consulta (verificar Paciente): ") + sqlite3_errmsg(db));
        }

        sqlite3_bind_int(stmt, 1, pessoaId);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Paciente já existe - atualiza
            int pacienteId = sqlite3_column_int(stmt, 0);
            sqlite3_finalize(stmt);
            stmt = nullptr;

            const char* sqlUpdatePaciente = "UPDATE Paciente SET TipoSanguineo = ?, TipoDiabetes = ? WHERE Id = ?";
            
            if (sqlite3_prepare_v2(db, sqlUpdatePaciente, -1, &stmt, nullptr) != SQLITE_OK) {
                throw std::runtime_error(std::string("Erro ao preparar UPDATE (Paciente): ") + sqlite3_errmsg(db));
            }

            sqlite3_bind_text(stmt, 1, this->bloodType.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, this->diabetesType.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 3, pacienteId);

            if (sqlite3_step(stmt) != SQLITE_DONE) {
                throw std::runtime_error(std::string("Erro ao executar UPDATE (Paciente): ") + sqlite3_errmsg(db));
            }

            std::cout << "Dados do paciente atualizados. ID: " << pacienteId << std::endl;
            sqlite3_finalize(stmt);
        } else {
            // Paciente não existe - insere novo
            sqlite3_finalize(stmt);
            stmt = nullptr;

            const char* sqlInsertPaciente = "INSERT INTO Paciente (Pessoa, TipoSanguineo, TipoDiabetes) VALUES (?, ?, ?)";
            
            if (sqlite3_prepare_v2(db, sqlInsertPaciente, -1, &stmt, nullptr) != SQLITE_OK) {
                throw std::runtime_error(std::string("Erro ao preparar INSERT (Paciente): ") + sqlite3_errmsg(db));
            }

            sqlite3_bind_int(stmt, 1, pessoaId);
            sqlite3_bind_text(stmt, 2, this->bloodType.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, this->diabetesType.c_str(), -1, SQLITE_TRANSIENT);

            if (sqlite3_step(stmt) != SQLITE_DONE) {
                throw std::runtime_error(std::string("Erro ao executar INSERT (Paciente): ") + sqlite3_errmsg(db));
            }

            int pacienteId = static_cast<int>(sqlite3_last_insert_rowid(db));
            std::cout << "Novo paciente inserido. ID: " << pacienteId << std::endl;
            sqlite3_finalize(stmt);
        }

        sqlite3_close(db);
        std::cout << "Paciente salvo no banco de dados com sucesso!" << std::endl;

    } catch (const std::exception& e) {
        // Tratamento de exceções - sempre limpa recursos
        std::cerr << "Exceção capturada em saveToDB: " << e.what() << std::endl;
        if (stmt) {
            sqlite3_finalize(stmt);
        }
        if (db) {
            sqlite3_close(db);
        }
        throw;
    } catch (...) {
        // Catch genérico pra erros não esperados
        std::cerr << "Erro desconhecido ao salvar paciente." << std::endl;
        if (stmt) {
            sqlite3_finalize(stmt);
        }
        if (db) {
            sqlite3_close(db);
        }
        throw;
    }
}