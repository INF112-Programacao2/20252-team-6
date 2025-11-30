#include "Person.hpp"
#include <iostream>
#include <stdexcept>

// Construtor - inicializa tudo e valida os dados
// Se algo estiver errado, lança exceção
Person::Person(std::string name, std::string cpf, std::string adress,
               std::string gender, int age, std::string password) : cpf(cpf), name(name), gender(gender), age(age), adress(adress), password(password) 
{
    // Validações básicas - não aceita dados inválidos
    if (name.empty()) {
        throw std::invalid_argument("Nome não pode ser vazio.");
    }
    if (cpf.empty()) {
        throw std::invalid_argument("CPF não pode ser vazio.");
    }
    if (age < 0) {
        throw std::invalid_argument("Idade não pode ser negativa.");
    }
    if (password.empty()) {
        throw std::invalid_argument("Senha não pode ser vazia.");
    }
}

// Getters simples - só retornam os valores
std::string Person::getName() const { return this->name; }
std::string Person::getCpf() const { return this->cpf; }
std::string Person::getAdress() const { return this->adress; }
std::string Person::getGender() const { return this->gender; }
int Person::getAge() const { return this->age; }
std::string Person::getPassword() const { return this->password; }

// Printa todas as infos da pessoa de forma organizada
void Person::printInfo() const
{
  std::cout << "Nome: " << this->name << "\n";
  std::cout << "CPF: " << this->cpf << "\n";
  std::cout << "Endereço: " << this->adress << "\n";
  std::cout << "Gênero: " << this->gender << "\n";
  std::cout << "Idade: " << this->age << "\n";
}
