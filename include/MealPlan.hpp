#ifndef MEALPLAN_HPP
#define MEALPLAN
#include<string>
#include"Patient.hpp"
#include<sqlite3.h>

class MealPlan{
    private:
    std::string foodAvoided;
    std::string nutricionist;
    std::string vitamins;
    int protein;
    int carbohydrate;
    int fat;
    Patient patient;
    public:
    MealPlan(Patient patient, int id);
    MealPlan(std::string foodAvoided, std::string nutricionist, std::string vitamins,
    int protein, int carbohydrate, int fat, Patient patient);
    ~MealPlan();
    void register_mealPlan(int id);
    void change_mealPlan(Patient patient);
};
#endif