#include <iostream>
#include "../include/Time.hpp"
#include <stdexcept>
#include <sstream>
#include <iomanip>

// Construtor - valida os valores antes de criar
Time::Time(int hour, int minute, int second)
{
  if (!isValid(hour, minute, second))
  {
    throw std::invalid_argument("Valores de tempo inválidos.");
  }
  this->hour = hour;
  this->minute = minute;
  this->second = second;
}
// Construtor a partir de string no formato "HH:MM" ou "HH:MM:SS"
Time::Time(const std::string& timeStr)
{
  std::istringstream iss(timeStr);
  std::string token;
  int h = 0, m = 0, s = 0;
  
  try {
    // Lê a hora
    if (!std::getline(iss, token, ':')) {
      throw std::invalid_argument("Formato de hora inválido. Use HH:MM ou HH:MM:SS");
    }
    h = std::stoi(token);
    
    // Lê o minuto
    if (!std::getline(iss, token, ':')) {
      throw std::invalid_argument("Formato de hora inválido. Use HH:MM ou HH:MM:SS");
    }
    m = std::stoi(token);
    
    // Tenta ler o segundo (opcional)
    if (std::getline(iss, token, ':')) {
      s = std::stoi(token);
    }
  } catch (const std::invalid_argument& e) {
    throw std::invalid_argument("Formato de hora inválido. Use HH:MM ou HH:MM:SS");
  } catch (const std::out_of_range& e) {
    throw std::invalid_argument("Valores de hora muito grandes.");
  }
  
  // Valida e atribui usando o método isValid
  if (!isValid(h, m, s)) {
    throw std::invalid_argument("Valores de tempo inválidos.");
  }
  
  this->hour = h;
  this->minute = m;
  this->second = s;
}

Time::~Time(){
}

// Valida se os valores estão no range correto
bool Time::isValid(int hour, int minute, int second)
{
  return (hour >= 0 && hour < 24) && (minute >= 0 && minute < 60) && (second >= 0 && second < 60);
}

// Getters simples
int Time::getHour() const
{
  return this->hour;
}
int Time::getMinute() const
{
  return this->minute;
}
int Time::getSecond() const
{
  return this->second;
}

// Setters - validam antes de setar
void Time::setHour(int hour)
{
  if (!isValid(hour, this->minute, this->second)) {
    throw std::invalid_argument("Hora inválida. Deve estar entre 0 e 23.");
  }
  this->hour = hour;
}
void Time::setMinute(int minute)
{
  if (!isValid(this->hour, minute, this->second)) {
    throw std::invalid_argument("Minuto inválido. Deve estar entre 0 e 59.");
  }
  this->minute = minute;
}
void Time::setSecond(int second)
{
  if (!isValid(this->hour, this->minute, second)) {
    throw std::invalid_argument("Segundo inválido. Deve estar entre 0 e 59.");
  }
  this->second = second;
}

// Mostra horário em formato 12h (AM/PM)
void Time::displayTime12() const
{
  int h12 = this->hour % 12;
  if (h12 == 0) h12 = 12; // Meia-noite e meio-dia são 12
  
  std::string period = (this->hour < 12) ? "AM" : "PM";
  
  std::cout << (h12 < 10 ? "0" : "") << h12 << ":"
            << (minute < 10 ? "0" : "") << this->minute << ":"
            << (second < 10 ? "0" : "") << this->second << " " << period << std::endl;
}

// Mostra horário em formato 24h
void Time::displayTime24() const
{
  std::cout << (hour < 10 ? "0" : "") << this->hour << ":"
            << (minute < 10 ? "0" : "") << this->minute << ":"
            << (second < 10 ? "0" : "") << this->second << std::endl;
}

// Converte Time pra string (formato HH:MM:SS)
std::string Time::toString() const
{
  std::ostringstream oss;
  oss << (hour < 10 ? "0" : "") << this->hour << ":"
      << (minute < 10 ? "0" : "") << this->minute << ":"
      << (second < 10 ? "0" : "") << this->second;
  return oss.str();
}
