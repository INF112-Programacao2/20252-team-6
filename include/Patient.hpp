#ifndef PATIENT_HPP
#define PATIENT_HPP
#include "Person.hpp"

// Classe que representa um paciente (herda de Person)
// Adiciona dados médicos específicos e métodos de login/agendamento
class Patient : public Person
{
private:
  const std::string diabetesType;  // Tipo de diabetes (tipo 1, tipo 2, pré-diabetes, etc)
  const std::string bloodType;     // Tipo sanguíneo (A+, O-, etc)
  double weight;                    // Peso em kg (pode mudar)
  double height;                    // Altura em metros (pode mudar)

public:
  // Construtor - recebe dados de Person + dados médicos
  Patient(std::string name, std::string cpf, std::string adress, std::string gender,
          int age, std::string password, std::string diabetesType, std::string bloodType, double weight, double height);
  virtual ~Patient() = default;
  
  // Getters dos dados médicos
  std::string getDiabetesType() const;
  std::string getBloodType() const;
  double getWeight() const;
  double getHeight() const;
  
  void setWeight(double weight);
  void setHeight(double height);

  static bool isValidBloodType(const std::string& bloodType);
  
  // Login no sistema - verifica CPF e senha no banco
  bool login(std::string inputCpf, std::string inputPassword);
  
  // Métodos de agendamento e cancelamento
  void makeAppointment(std::string date, std::string hour, std::string doctor, std::string specialty, std::string description, std::string location);
  void cancelAppointment(int appointmentId);
  void bookExam(std::string date, std::string hour, std::string nameExam, std::string doctor, std::string lab, std::string result);
  void cancelExam(int examId);
  
  // Mostra estado clínico do paciente
  void printClinicalState() const;
  
  // Registra dados clínicos (peso, altura, etc)
  void registerClinicalData(std::string date, std::string hour, std::string description);
  
  // Imprime todos os registros do paciente
  void imprimirRegister() const;
  
  // Busca o ID do paciente no banco (usando nome e CPF)
  int searchId();
  
  // Salva o paciente no banco de dados (insere/atualiza Pessoa e Paciente)
  void saveToDB();
  
  // Método estático pra verificar login sem criar objeto
  static bool verifyLogin(const std::string& inputCpf, const std::string& inputPassword);

  // Metodo estatico para carregar as informacoes do banco de dados
  static Patient* loadFromDB(const std::string& cpf);
};

#endif