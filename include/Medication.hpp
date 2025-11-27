#ifndef MEDICATION_HPP
#define MEDICATION_HPP
#include <string>
#include "Time.hpp"

class Medication
{
private:
  int id;
  int patientId;
  const std::string name;
  const std::string activeIngredient;
  Time timeMedication;
  double dosage;
  const std::string doctor;

public:
  Medication(int patientId, std::string name, std::string activeIngredient, Time timeMedication, double dosage, std::string doctor);
  Medication(int id, int patientId, std::string name, std::string activeIngredient, Time timeMedication, double dosage, std::string doctor);
  ~Medication();
  void printInfo() const;
  std::string getName() const;
  std::string getActiveIngredient() const;
  double getDosage() const;
  int getId() const;
  int getPatientId() const;
  std::string getDoctor() const;
  Time getTimeMedication() const;
  void setDosage(double dosage);
  void saveToDB();
};

#endif