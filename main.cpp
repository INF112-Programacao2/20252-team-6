#include <iostream>
#include <sqlite3.h>
#include <limits>
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
            std::cout << "===========================\n";
            std::cout << "Credenciais Validadas. Carregando dados completos...\n";
            std::cout << "===========================\n";
            
            // Aqui carregamos o paciente REAL
            paciente_real = Patient::loadFromDB(cpf); 

            if (paciente_real == nullptr) {
                std::cout << "Falha: O usuário não é um Paciente ou erro no DB.\n";
            }
            
        } else {
            std::cout << "\nFalha na tentativa de login.\n";
        }
     
    //ID do paciente logado
    int ID = paciente_real->searchId(); 
    
    int choice;
    while (true){
        std::cout << "==============================\n";
        std::cout << "Selecione o que quer fazer:   \n";
        std::cout << "1) Marcar uma consulta        \n";
        std::cout << "2) Exibir consultas marcadas  \n";
        std::cout << "3) Marcar um exame            \n";
        std::cout << "4) Exibir exames marcados     \n";
        std::cout << "5) Registrar nivel de glicose \n";
        std::cout << "6) Exibir registros de glicose\n";
        std::cout << "7) Registrar plano alimentar  \n";
        std::cout << "8) Mudar plano alimentar      \n";
        if(std::cin >> choice){
            break;
        } else {
            std::cout << "Escolha uma opcao valida!\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

        
        if (paciente_real != nullptr)
            delete paciente_real;

    return 0;
}
