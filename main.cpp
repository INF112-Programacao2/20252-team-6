#include <iostream>
#include <sqlite3.h>
#include "include/MealPlan.hpp"
#include "include/Patient.hpp"
#include "include/Person.hpp"

int main(){
    Patient Davi("Davi", "12345678901", "Rua Presidente Carvalho, 175", "Masculino", 12345, "pre-diabetes", "O-", 80, 1.70);
    MealPlan mp("Refrigerante, chocolate", "Sandra", "B12, A", 120, 200, 20, Davi);
    int id=1;
    mp.register_mealPlan(id);
    return 0;
}
