#include "../include/Patient.hpp"
#include "../include/Person.hpp"
#include <iostream>
#include <sqlite3.h>
#include <algorithm>
#include <cctype>
#include <locale>
#include <string>

Patient::Patient(std::string name, std::string cpf, std::string adress,
                 std::string gender, int age, std::string diabetesType, std::string bloodType,
                 double weight, double height)
    : Person(name, cpf, adress, gender, age), diabetesType(diabetesType), bloodType(bloodType),
      weight(weight), height(height) {}
Patient::~Patient() {}

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
void Patient::makeAppointment()
{
  std::cout << "makeAppointment \n";
}
void Patient::cancelAppointment()
{
  std::cout << "cancelAppointment \n";
}
void Patient::bookExam()
{
  std::cout << "bookExam \n";
}
void Patient::cancelExam()
{
  std::cout << "cancelExam \n";
}
void Patient::printClinicalState()
{
  std::cout << "printClinicalState() \n";
}
void Patient::registerClinicalData()
{
  std::cout << "registerClincalData \n";
}
void Patient::imprimirRegister()
{
  std::cout << "printRegister \n";
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

int Patient::searchId(Patient& patient){
    sqlite3* db;
    sqlite3_stmt* stmt = nullptr; // Sempre inicialize para segurança

    int rc_open = sqlite3_open("database.db", &db);
    if (rc_open != SQLITE_OK) { // Se a abertura do banco falhar
        std::cerr << "Erro ao abrir banco: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db); 
        return -1;
    }
    
    // Altere para SELECT id para buscar apenas a coluna 0 que é o ID
    const char* sql = "SELECT id FROM Pessoa WHERE Nome = ? AND Cpf = ?"; 

    int idPerson=-1;
    int idPatient=-1;

    // --- 1. BUSCA POR PESSOA ---
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        //limpar as strings antes de vincular
        std::string cleanName = trim(patient.getName());
        std::string cleanCpf = trim(patient.getCpf());
        
        //vincular o parâmetro (?) ao valor da variavel
        sqlite3_bind_text(stmt, 1, cleanName.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, cleanCpf.c_str(), -1, SQLITE_STATIC);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            idPerson = sqlite3_column_int(stmt, 0); // Coluna 0 é o ID
            std::cout << "Pessoa ID: " << idPerson << std::endl;
        } else {
            // Se não encontrou, idPerson continua sendo -1
            std::cout << "Pessoa nao encontrada para Nome/Cpf: " <<  cleanName<< " / " << cleanCpf << std::endl;
        }
        sqlite3_finalize(stmt); // finaliza o statement da primeira query
        stmt = nullptr; // reseta stmt
    } 
    else 
    {
        std::cerr<<"Erro ao preparar SELECT (Pessoa): "<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_close(db);
        return -1;
    }

    // --- CORREÇÃO CRÍTICA: SAÍDA ANTECIPADA ---
    if (idPerson == -1) {
        // Se a pessoa não foi encontrada, a busca do Paciente é impossível.
        sqlite3_close(db);
        return -1;
    }


    // --- 2. BUSCA POR PACIENTE ---
    // Busca o ID do Paciente (coluna 0)
    const char* sql2 = "SELECT Id FROM Paciente WHERE Pessoa = ?"; 

    if (sqlite3_prepare_v2(db, sql2, -1, &stmt, nullptr) == SQLITE_OK) {
        // Vincular ao idPerson encontrado
        sqlite3_bind_int(stmt, 1, idPerson);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            idPatient = sqlite3_column_int(stmt, 0); // Coluna 0 é o ID
            std::cout << "Paciente ID: " << idPatient << std::endl;
        } else {
            std::cout << "Paciente nao encontrado para Pessoa ID: " << idPerson << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Erro ao preparar SELECT (Paciente): " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_close(db);

    return idPatient;
}