#ifndef MEDICATION_RECORD_HPP
#define MEDICATION_RECORD_HPP
#include "HealthRecord.hpp"
#include "Medication.hpp"
#include <string>

// Classe que representa um registro de uso de medicamento
// Herda de HealthRecord e tem uma referência pro medicamento usado
// Relação de agregação: MedicationRecord "tem um" Medication
class MedicationRecord : public HealthRecord
{
private:
    const Medication& medication;  // Referência pro medicamento que foi tomado

public:
    // Construtor - recebe paciente, data/hora e o medicamento
    MedicationRecord(const Patient& patient, std::string date, Time hour, const Medication& medication);
    virtual ~MedicationRecord();
    
    // Salva o registro no banco (primeiro em RegistroSaude, depois em RegistroMedicacao)
    virtual void registerDB(int patientId) override;
    
    // Mostra os detalhes do registro
    virtual void displayDetails() override;
    
    // Getter pro medicamento
    const Medication& getMedication() const;
    
    // Carrega registro do banco pelo ID
    static MedicationRecord* loadFromDB(int recordID);
};

#endif

