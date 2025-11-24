#ifndef PERSON_HPP
#define PERSON_HPP
#include <string>

class Person
{
private:
  const std::string cpf;
  const std::string name;
  const std::string gender;
  int age;
  const std::string adress;
  const std::string password;

public:
  Person(std::string name, std::string cpf, std::string adress,
         std::string gender, int age, std::string password);
  Person(Person &&) = default;
  Person(const Person &) = default;
  Person &operator=(Person &&) = default;
  Person &operator=(const Person &) = default;
  virtual ~Person() = default;
  virtual std::string getName() const;
  virtual std::string getCpf() const;
  virtual std::string getAdress() const;
  virtual std::string getGender() const;
  virtual int getAge() const;
  virtual std::string getPassword() const;
  void printInfo() const;
};

#endif
