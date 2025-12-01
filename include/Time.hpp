#ifndef TIME_HPP
#define TIME_HPP

#include <string>

// Classe pra representar um horário (hora, minuto, segundo)
// Valida os valores pra garantir que estão no range correto
class Time
{
private:
  unsigned short int hour;      // Hora (0-23)
  unsigned short int minute;    // Minuto (0-59)
  unsigned short int second;    // Segundo (0-59)
  
public:

  // Método estático pra validar se os valores são válidos
  static bool isValid(int hour, int minute, int second);

  // Metodo para verificar formato
  bool isStringValid( const std::string& string); 

  // Construtor - valida e lança exceção se inválido
  Time(int hour, int minute, int second);
  
  // Construtor a partir de string no formato "HH:MM" ou "HH:MM:SS"
  Time(const std::string& timeStr);
  
  ~Time();
  
  // Getters
  int getHour() const;
  int getMinute() const;
  int getSecond() const;
  
  // Setters - validam antes de setar (lançam exceção se inválido)
  void setHour(int hour);
  void setMinute(int minute);
  void setSecond(int second);
  
  // Mostra o horário em formato 24h ou 12h
  void displayTime24() const;
  void displayTime12() const;
  
  // Converte Time pra string (formato HH:MM:SS)
  std::string toString() const;
};

#endif