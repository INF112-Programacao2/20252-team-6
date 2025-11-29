#include <iostream>
#include <sqlite3.h>
#include "include/MealPlan.hpp"
#include "include/Patient.hpp"
#include "include/Person.hpp"
#include "include/HealthRecord.hpp"
#include "include/ExamRecord.hpp"

int main(){
    Patient Pedro("Pedro Tavares", "98765432101", "Rua Presidente II, 675", "Marculino", 40, "54321", "Diabetes", "AB+", 90, 1.80);
    MealPlan mp("Refrigerante, chocolate", "Sandra", "B12, C", 120, 200, 20, Pedro);
    int id = Pedro.searchId();
    mp.register_mealPlan(id);
    mp.change_mealPlan(id);
    //HealthRecord hr(Pedro, "18/11/2025", "01:00");
    //hr.registerDB(id);
    ExamRecord er(Pedro, "19/11/2025", "08:00", "Hemograma", "alta '%' de acucar no sangue", "Lab Central", "Carlos Santos");
    er.registerDB(id);
    return 0;
}
