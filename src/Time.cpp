#include <iostream>
#include "../include/Time.hpp"
#include <stdexcept>

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

// Setters - validam antes de setar (usam isValid pra garantir consistência)
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
  std::cout << (hour < 10 ? "0" : "") << this->hour % 12 << ":"
            << (minute < 10 ? "0" : "") << this->minute << ":"
            << (second < 10 ? "0" : "") << this->second << std::endl;
}

// Mostra horário em formato 24h
void Time::displayTime24() const
{
  std::cout << (hour < 10 ? "0" : "") << this->hour << ":"
            << (minute < 10 ? "0" : "") << this->minute << ":"
            << (second < 10 ? "0" : "") << this->second << std::endl;
}
