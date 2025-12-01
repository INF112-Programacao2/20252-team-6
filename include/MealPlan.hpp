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
    const Patient& patient;
    public:
    MealPlan(std::string foodAvoided, std::string nutricionist, std::string vitamins,
    int protein, int carbohydrate, int fat, const Patient& patient);
    ~MealPlan();
    void register_mealPlan(int id);
    void change_mealPlan(int id);
};
#endif