#include <iostream>
#include "Time.hpp"

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
bool Time::isValid(int hour, int minute, int second)
{
  return (hour >= 0 && hour < 24) && (minute >= 0 && minute < 60) && (second >= 0 && second < 60);
}
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
void Time::setHour(int hour)
{
  if (hour >= 0 && hour < 24)
    this->hour = hour;
}
void Time::setMinute(int minute)
{
  if (minute >= 0 && minute < 60)
    this->minute = minute;
}
void Time::setSecond(int second)
{
  if (second >= 0 && second < 60)
    this->second = second;
}
void Time::displayTime12() const
{
  std::cout << (hour < 10 ? "0" : "") << this->hour % 12 << ":"
            << (minute < 10 ? "0" : "") << this->minute << ":"
            << (second < 10 ? "0" : "") << this->second << std::endl;
}
void Time::displayTime24() const
{
  std::cout << (hour < 10 ? "0" : "") << this->hour << ":"
            << (minute < 10 ? "0" : "") << this->minute << ":"
            << (second < 10 ? "0" : "") << this->second << std::endl;
}
