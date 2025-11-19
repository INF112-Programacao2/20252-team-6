#ifndef HEALTH_RECORD_HPP
#define HEALTH_RECORD_HPP
#include <string>
#include "Patient.hpp"

class HealthRecord
{
private:
    Patient patient;
    std::string date;
    std::string hour;
public:
    HealthRecord(Patient patient, std::string date, std::string hour);
    virtual ~HealthRecord();
    virtual void  registerDB(int id);
    virtual void displayDetails()=0;
    std::string getHour();
    std::string getDate();
};

#endif
