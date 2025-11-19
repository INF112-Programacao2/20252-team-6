#include <iostream>
#include <sqlite3.h>
#include "include/MealPlan.hpp"
#include "include/Patient.hpp"
#include "include/Person.hpp"
#include "include/HealthRecord.hpp"
#include "include/ExamRecord.hpp"

int main(){
    Patient Pedro("Pedro Alves", "12345678902", "Rua Presidente Carvalho, 175",
    "Masculino", 12345, "pre-diabetes", "O-", 80, 1.70);
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
