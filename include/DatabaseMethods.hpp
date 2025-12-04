#ifndef DATABASE_METHODS_HPP
#define DATABASE_METHODS_HPP

class DatabaseMethods
{
private:
public:
    DatabaseMethods();
    ~DatabaseMethods();
    void displayDetailsExamRecordDB(int id);
    void displayDetailsConsultationRecordDB(int id);
    void displayDetailsGlucoseRecordDB(int id);
    void displayDetailsMedicationRecordDB(int id);
    bool createPatient();
    void displayMedications(int id);
    bool isValidAge(const std::string& ageStr);
    bool isValidName(const std::string& name);
    bool isValidWeight(const std::string& weightStr);
    bool isValidHeight(const std::string& heightStr);
    bool isValidCPF(std::string& cpfStr);
    bool isValidBloodType(const std::string& bloodType);
    bool isValidDateString(const std::string& date);
    bool isDateTimeNotFuture(const std::string& date, const std::string& time);
    bool isDateTimeNotPast(const std::string& date, const std::string& time);
};
#endif

