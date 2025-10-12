#ifndef PATIENT_HPP
#define PATIENT_HPP
#include "Person.hpp"
class Patient : public Person
{
private:
  const std::string diabetesType;
  const std::string bloodType;
  double weight;
  double height;

public:
  Patient(std::string name, std::string cpf, std::string adress, std::string gender,
          int age, std::string diabetesType, std::string bloodType);
  ~Patient();
  std::string getDiabetesType();
  std::string getBloodType();
  double getWeight();
  double getHeight();
  void makeAppointment();
  void cancelAppointment();
  void bookExam();
  void cancelExam();
  void printClinicalState();
  void registerClinicalData();
  void imprimirRegister();
};

#endif