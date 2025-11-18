#include <iostream>
#include <sqlite3.h>
#include "include/MealPlan.hpp"
#include "include/Patient.hpp"
#include "include/Person.hpp"

int main(){
    Patient Pedro("Pedro Alves", "12345678902", "Rua Presidente Carvalho, 175",
    "Masculino", 12345, "pre-diabetes", "O-", 80, 1.70);
    MealPlan mp("Refrigerante, chocolate", "Sandra", "B12, C", 120, 200, 20, Pedro);
    int id=1;
    mp.register_mealPlan(id);
    mp.change_mealPlan(id);
    //testando função de buscar id
    std::cout<<Pedro.searchId();
    return 0;
}
