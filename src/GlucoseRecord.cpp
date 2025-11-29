#include <iostream>
#include <string>
#include "../include/Patient.hpp"
#include "../include/GlucoseRecord.hpp"

GlucoseRecord::GlucoseRecord(const Patient& patient, std::string date, std::string hour, int glucoseLevel, bool fast)
    :HealthRecord(patient, date, hour), glucoseLevel(glucoseLevel), fast(fast){}

GlucoseRecord::~GlucoseRecord(){}

int GlucoseRecord::getGlucoseLevel(){
    return glucoseLevel;
}

bool GlucoseRecord::getFast(){
    return fast;
}

void GlucoseRecord::displayDetails(){
    std::cout << "Valores aproximados";
}