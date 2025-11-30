#ifndef HEALTH_RECORD_HPP
#define HEALTH_RECORD_HPP
#include <string>
#include "Patient.hpp"
#include "Time.hpp"

class HealthRecord
{
private:
    const Patient& patient;
    std::string date;
    Time hour;
public:
    HealthRecord(const Patient& patient, std::string date, Time hour);
    virtual ~HealthRecord();
    virtual void  registerDB(int id);
    virtual void displayDetails();
    virtual void displayDetailsDB(int id);
    Time getHour() const;
    std::string getHourString() const;
    std::string getDate() const;
    Patient getPatient() const;
};

#endif
