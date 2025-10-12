#include <iostream>
#include "Medication.hpp"

Medication::Medication(std::string name, std::string activeIngredient, Time timeMedication,
                       double dosage) : name(name), activeIngredient(activeIngredient),
                                        timeMedication(timeMedication), dosage(dosage)
{
}
Medication::~Medication(){}
void Medication::printInfo()
{
  std::cout << "Nome: " << this->name << "\n";
  std::cout << "Principio Ativo: " << this->activeIngredient << "\n";
  this->timeMedication.displayTime24();
  std::cout<<"\n";
  std::cout << "Dosagem: " << this->dosage << "\n";
}
std::string Medication::getName() const
{
  return this->name;
}
std::string Medication::getActiveIngredient() const
{
  return this->activeIngredient;
}
double Medication::getDosage() const
{
  return this->dosage;
}
void Medication::setDosage(double dosage)
{
  this->dosage = dosage;
}