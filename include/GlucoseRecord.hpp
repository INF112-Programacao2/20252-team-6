#ifndef GLUCOSE_RECORD_HPP
#define GLUCOSE_RECORD_HPP
#include "HealthRecord.hpp"
#include "Patient.hpp"

class GlucoseRecord : public HealthRecord{
    private:
    int glucoseLevel;
    bool fast;//jejum
    public:
    GlucoseRecord(const Patient& patient, std::string date, std::string hour, int glucoseLevel, bool fast);
    virtual ~GlucoseRecord();
    void displayDetails() override;
    void registerDB(int id) override;
    int getGlucoseLevel();
    bool getFast();
};
#endif