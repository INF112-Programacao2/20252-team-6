#include "Person.hpp"
#include <iostream>

Person::Person(std::string name, std::string cpf, std::string adress,
               std::string gender, int age) : cpf(cpf), name(name), gender(gender), age(age), adress(adress) {}

Person::~Person() {}
std::string Person::getName() const { return this->name; }
std::string Person::getCpf() const { return this->cpf; }
std::string Person::getAdress() const { return this->adress; }
std::string Person::getGender() const { return this->gender; }
int Person::getAge() { return this->age; }
void Person::printPersonalInfo() const
{
  std::cout << "Nome: " << this->name << "\n";
  std::cout << "CPF: " << this->cpf << "\n";
  std::cout << "Endereço: " << this->adress << "\n";
  std::cout << "Gênero: " << this->gender << "\n";
  std::cout << "Idade: " << this->age << "\n";
}
