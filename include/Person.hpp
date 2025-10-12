#ifndef PERSON_HPP
#define PERSON_HPP
#include <map>
#include <string>

class Person
{
private:
  const std::string cpf;
  const std::string name;
  const std::string gender;
  int age;
  const std::string adress;
  const std::map<std::string, std::string> loginPassword;

public:
  Person(std::string name, std::string cpf, std::string adress,
         std::string gender, int age);
  Person(Person &&) = default;
  Person(const Person &) = default;
  Person &operator=(Person &&) = default;
  Person &operator=(const Person &) = default;
  virtual ~Person();
  virtual std::string getName() const;
  virtual std::string getCpf() const;
  virtual std::string getAdress() const;
  virtual std::string getGender() const;
  virtual int getAge() const;
  void printInfo() const;
};

#endif
