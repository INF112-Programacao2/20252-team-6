#ifndef EXAM_RECORD_HPP
#define EXAM_RECORD_HPP
#include<string>
#include"HealthRecord.hpp"

class ExamRecord : public HealthRecord
{
private:
    std::string nameExam;
    std::string result;
    std::string lab;
    std::string doctor;
public:
    ExamRecord(const Patient& patient, std::string date, std::string hour, std::string nameExam,
    std::string result, std::string lab, std::string doctor);
    virtual ~ExamRecord();
    void displayDetails() override;
    void registerDB(int id) override;
    std::string getName();
    std::string getResult();
    std::string getLab();
    std::string getDoctor();
};

#endif