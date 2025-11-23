#ifndef CONSULTATION_RECORD_HPP
#define CONSULTATION_RECORD_HPP
#include "HealthRecord.hpp"
#include "Patient.hpp"
#include <string>

class ConsultationRecord : public HealthRecord{
    private:
    std::string doctor;
    std::string specialty;
    std::string descrition;
    std::string location;
    public:
    ConsultationRecord(const Patient& patient, std::string date, std::string hour,
    std::string doctor, std::string specialty, std::string descrition, std::string location);
    virtual ~ConsultationRecord();
    void displayDetails() override;
    void registerDB(int id) override;
    std::string getDoctor();
    std::string getSpecialty();
    std::string getDescrition();
    std::string getLocation();
};
#endif