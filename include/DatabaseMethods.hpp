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
};
#endif

