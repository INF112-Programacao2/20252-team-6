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
    
    std::string cpf;
    std::string senha;

    std::cout << "========== LOGIN ==========\n";
    std::cout << "Digite seu CPF: ";
    std::cin >> cpf;
    std::cout << "Digite sua senha: ";
    std::cin >> senha;

    // Tipo paciente para usar o metodo de verificacao de LOGIN
    Patient paciente_temporario {"-", cpf, "-", "-", 1, senha, "-", "-", 1.0, 1.0};
    Patient* paciente_real = nullptr;

    if (paciente_temporario.login(cpf, senha)) {
            std::cout << "\n====================================\n";
            std::cout << "Credenciais Validadas. Carregando dados completos...\n";
            std::cout << "====================================\n";
            
            // Aqui carregamos o paciente REAL
            paciente_real = Patient::loadFromDB(cpf); 

            if (paciente_real != nullptr) {
                std::cout << "\n--- Paciente Logado ---\n";
        
            } else {
                std::cout << "Falha: O usuário não é um Paciente ou erro no DB.\n";
            }
        } else {
            std::cout << "\nFalha na tentativa de login.\n";
        }
        
        
        // Terminar restante da main usando paciente_real!
        
        
        
        
        
        
        
        if (paciente_real != nullptr)
            delete paciente_real;


    return 0;
}
