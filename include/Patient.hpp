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
          int age, std::string password, std::string diabetesType, std::string bloodType, double weight, double height);
  virtual ~Patient() = default;
  std::string getDiabetesType() const;
  std::string getBloodType() const;
  double getWeight() const;
  double getHeight() const;
  bool login(std::string inputCpf, std::string inputPassword);
  void makeAppointment(std::string date, std::string hour, std::string doctor, std::string specialty, std::string description, std::string location);
  void cancelAppointment(int appointmentId);
  void bookExam(std::string date, std::string hour, std::string nameExam, std::string doctor, std::string lab, std::string result);
  void cancelExam(int examId);
  void printClinicalState() const;
  void registerClinicalData(std::string date, std::string hour, std::string description);
  void imprimirRegister() const;
  int searchId();
  static bool verifyLogin(const std::string& inputCpf, const std::string& inputPassword);
};

#endif