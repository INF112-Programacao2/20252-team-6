#include "Patient.hpp"
#include <iostream>
Patient::Patient(std::string name, std::string cpf, std::string adress,
                std::string gender, int age, std::string diabetesType, std::string bloodType)
    : Person(name, cpf, adress, gender, age), diabetesType(diabetesType), bloodType(bloodType) {}
Patient::~Patient() {}
std::string Patient::getDiabetesType()
{
  return this->diabetesType;
}
std::string Patient::getBloodType()
{
  return this->bloodType;
}
double Patient::getWeight()
{
  return this->weight;
}
double Patient::getHeight()
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
