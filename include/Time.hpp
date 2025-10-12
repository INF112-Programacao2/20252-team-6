#ifndef TIME_HPP
#define TIME_HPP
class Time
{
private:
  unsigned short int hour;
  unsigned short int minute;
  unsigned short int second;
  static bool isValid(int hour, int minute, int second);

public:
  Time(int hour, int minute, int second);
  ~Time();
  int getHour() const;
  int getMinute() const;
  int getSecond() const;
  void setHour(int hour);
  void setMinute(int minute);
  void setSecond(int second);
  void displayTime24() const;
  void displayTime12() const;
};

#endif