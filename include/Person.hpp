#ifndef PERSON_HPP
#define PERSON_HPP
#include <string>

// Classe base para representar uma pessoa no sistema
// Guarda dados básicos como nome, CPF, endereço, etc.
class Person
{
private:
  const std::string cpf;        // CPF da pessoa (const pq não muda)
  const std::string name;        // Nome completo
  const std::string gender;      // Gênero
  int age;                       // Idade (não const pq pode mudar)
  const std::string adress;      // Endereço
  const std::string password;    // Senha para login

public:
  // Construtor principal - valida os dados antes de criar
  Person(std::string name, std::string cpf, std::string adress,
         std::string gender, int age, std::string password);
  // Construtores de movimento e cópia (default)
  Person(Person &&) = default;
  Person(const Person &) = default;
  Person &operator=(Person &&) = default;
  Person &operator=(const Person &) = default;
  // Destrutor virtual (importante pra herança)
  virtual ~Person() = default;
  
  // Getters básicos (virtual pra permitir override nas subclasses)
  virtual std::string getName() const;
  virtual std::string getCpf() const;
  virtual std::string getAdress() const;
  virtual std::string getGender() const;
  virtual int getAge() const;
  virtual std::string getPassword() const;
  void setAge(int age);
  
  // Imprime todas as infos da pessoa
  void printInfo() const;
};

#endif
