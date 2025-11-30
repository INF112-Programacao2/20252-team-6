#include "../include/Patient.hpp"
#include "../include/Person.hpp"
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
    
    std::cout << "Agendando consulta..." << std::endl;
    std::cout << "Data: " << date << std::endl;
    std::cout << "Hora: " << hour << std::endl;
    std::cout << "Médico: " << doctor << std::endl;
    std::cout << "Especialidade: " << specialty << std::endl;
    std::cout << "Descrição: " << description << std::endl;
    std::cout << "Local: " << location << std::endl;
    // TODO: Implementar criação de ConsultationRecord e registro no banco
}

// Cancela uma consulta agendada
void Patient::cancelAppointment(int appointmentId) {
    if (appointmentId <= 0) {
        throw std::invalid_argument("ID da consulta deve ser maior que zero.");
    }
    std::cout << "Cancelando consulta com ID: " << appointmentId << std::endl;
    // TODO: Implementar cancelamento da consulta no banco de dados
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
    
    std::cout << "Agendando exame..." << std::endl;
    std::cout << "Data: " << date << std::endl;
    std::cout << "Hora: " << hour << std::endl;
    std::cout << "Nome do exame: " << nameExam << std::endl;
    std::cout << "Médico: " << doctor << std::endl;
    std::cout << "Laboratório: " << lab << std::endl;
    std::cout << "Resultado: " << result << std::endl;
    // TODO: Implementar criação de ExamRecord e registro no banco
}

// Cancela um exame agendado
void Patient::cancelExam(int examId) {
    if (examId <= 0) {
        throw std::invalid_argument("ID do exame deve ser maior que zero.");
    }
    std::cout << "Cancelando exame com ID: " << examId << std::endl;
    // TODO: Implementar cancelamento do exame no banco de dados
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
    
    std::cout << "Registrando dados clínicos..." << std::endl;
    std::cout << "Data: " << date << std::endl;
    std::cout << "Hora: " << hour << std::endl;
    std::cout << "Descrição: " << description << std::endl;
    // TODO: Implementar registro de dados clínicos no banco de dados
}

// Imprime todos os registros do paciente
void Patient::imprimirRegister() const {
    std::cout << "Imprimindo registro do paciente..." << std::endl;
    this->printClinicalState();
    // TODO: Implementar impressão de registros completos do paciente
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

            sqlite3_bind_text(stmt, 1, this->bloodType.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, this->diabetesType.c_str(), -1, SQLITE_STATIC);
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
            sqlite3_bind_text(stmt, 2, this->bloodType.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, this->diabetesType.c_str(), -1, SQLITE_STATIC);

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