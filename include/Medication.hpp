#ifndef MEDICATION_HPP
#define MEDICATION_HPP
#include <string>
#include "Time.hpp"
class Medication
{
private:
  const std::string name;
  const std::string activeIngredient;
  Time timeMedication;
  double dosage;

public:
  Medication(std::string name, std::string activeIngredient, Time timeMedication, double dosage);
  ~Medication();
  void printInfo();
  std::string getName() const;
  std::string getActiveIngredient() const;
  double getDosage() const;
  void setDosage(double dosage);
};

#endif