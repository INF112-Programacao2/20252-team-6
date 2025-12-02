#ifndef MEDICATION_HPP
#define MEDICATION_HPP
#include <string>
#include <vector>

// Classe que representa um medicamento prescrito para um paciente
// Guarda nome, horário de administração, dosagem, etc.
class Medication
{
private:
  int id;                          // ID no banco (gerado automaticamente)
  int patientId;                   // ID do paciente que usa esse medicamento
  const std::string name;         // Nome do medicamento
  int timeMedication;              // Intervalo entre administrações (em horas)
  double dosage;                   // Dosagem (pode mudar)
  const std::string doctor;        // Nome do médico que prescreveu

public:
  // Construtor pra criar novo medicamento (id = -1 inicialmente)
  Medication(int patientId, std::string name, int timeMedication, double dosage, std::string doctor);
  // Construtor pra carregar medicamento do banco (com id já definido)
  Medication(int id, int patientId, std::string name, int timeMedication, double dosage, std::string doctor);
  ~Medication();
  
  // Mostra todas as infos do medicamento
  void printInfo() const;
  
  // Getters
  std::string getName() const;
  double getDosage() const;
  int getId() const;
  int getPatientId() const;
  std::string getDoctor() const;
  int getTimeMedication() const;
  
  // Setter da dosagem (valida antes de setar)
  void setDosage(double dosage);
  
  // Salva o medicamento no banco de dados
  void saveToDB();
  
  // Carrega medicamento do banco pelo ID
  static Medication* loadFromDB(int medicationID);
  
};

#endif