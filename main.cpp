#include <iostream>
#include <sqlite3.h>
#include "include/MealPlan.hpp"
#include "include/Patient.hpp"
#include "include/Person.hpp"
#include "include/HealthRecord.hpp"
#include "include/ExamRecord.hpp"
#include "include/ConsultationRecord.hpp"
#include "include/GlucoseRecord.hpp"
#include "include/DatabaseMethods.hpp"

int main(){
    Patient Pedro("Pedro Tavares", "98765432101", "Rua Presidente II, 675", "Marculino", 40, "54321", "Diabetes", "AB+", 90, 1.80);
    MealPlan mp("Refrigerante, chocolate", "Sandra", "B12, C", 120, 200, 20, Pedro);
    int id = Pedro.searchId();
    mp.register_mealPlan(id);
    mp.change_mealPlan(id);
    ExamRecord er(Pedro, "19/11/2025", "08:00", "Hemograma", "alta '%' de acucar no sangue", "Lab Central", "Carlos Almeida");
    er.registerDB(id);
    GlucoseRecord gr(Pedro, "11/11/2025", "11:11", 120, true);
    gr.registerDB(id);
    ConsultationRecord cr(Pedro, "25/11/2025", "10:00", "Arthur Cardoso",
        "Hematologia", "Problemas na cicatrizacao de feridas", "Clinica Silva");
    cr.registerDB(id);

    DatabaseMethods dm;
    dm.displayDetailsExamRecordDB(id);
    std::cout<<std::endl;
    dm.displayDetailsConsultationRecordDB(id);
    dm.displayDetailsGlucoseRecordDB(id);
    return 0;
}
