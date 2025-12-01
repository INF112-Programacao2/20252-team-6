#include <iostream>
#include <sqlite3.h>
#include <limits>
#include <string>
#include "include/MealPlan.hpp"
#include "include/Patient.hpp"
#include "include/Person.hpp"
#include "include/HealthRecord.hpp"
#include "include/ExamRecord.hpp"
#include "include/ConsultationRecord.hpp"
#include "include/GlucoseRecord.hpp"
#include "include/DatabaseMethods.hpp"
#include "include/Time.hpp"

int main(){
    
    std::string cpf;
    std::string senha;

    while(true){
        int escolha;
        std::cout << "===========================\n";
        std::cout << "Deseja fazer o que?        \n";
        std::cout << "1) Criar uma conta         \n";
        std::cout << "2) Login                   \n";
        std::cout << "3) Sair                    \n";
        std::cout << "===========================\n";
        
        if(std::cin >> escolha){
            if(escolha == 1){
                DatabaseMethods sign;
                sign.createPatient();
            } else if(escolha == 2){
                break;
            } else if(escolha == 3){
                return 0;
            } else {
                std::cout << "Digite uma opcao valida!\n";
            }
        } else {
            std::cout << "Digite uma opcao valida!\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    std::cout << "========== LOGIN ==========\n";
    std::cout << "Digite seu CPF: ";
    std::cin >> cpf;
    std::cout << "Digite sua senha: ";
    std::cin >> senha;

    // Tipo paciente para usar o metodo de verificacao de LOGIN
    Patient paciente_temporario {"-", cpf, "-", "-", 1, senha, "-", "A-", 1.0, 1.0};
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
    // Loop principal de exibicao da tabela
    bool continuous = true;
    while (continuous){
        // Loop para repitir ate o usario inserir um valor valido
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
            std::cout << "9) Exibir plano alimentar     \n";
            std::cout << "10) Sair                      \n";
            std::cout << "==============================\n";
            if(std::cin >> choice){
                break;
            } else {
                std::cout << "Escolha uma opcao valida!\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }

        switch (choice){

            // Marcar Consulta
            case 1: {
                std::string localizacao;
                std::cout << "Deseja marcar em qual local?\n";
                std::cin.ignore();
                std::getline(std::cin, localizacao);
                
                std::string area;
                std::cout << "Deseja marcar com que especialista?\n";
                std::getline(std::cin,area);

                std::string nome;
                std::cout << "Qual o nome do medico?\n";
                std::getline(std::cin,nome);

                std::string data;
                std::cout << "Deseja marcar em que data?\n";
                std::getline(std::cin,data);

                std::string hora;
                while(true){
                    std::cout << "Deseja marcar que horas? (HH:MM)\n";
                    std::getline(std::cin,hora);
                    Time Verificacao(1,1,1);
                    if(Verificacao.isStringValid(hora)){
                        break;
                    } else {
                        std::cout << "Digite um formato valido! (HH:MM)\n";
                    }
                }

                std::string descricao;
                std::cout << "Adicione uma breve descricao\n";
                std::getline(std::cin, descricao);

                Time horas(hora);
                ConsultationRecord consulta(*paciente_real, data, horas, nome, area, descricao, localizacao);
                consulta.registerDB(ID);

                break;
            }
            
            // Exibir Consultas
            case 2: {
                DatabaseMethods exibir;
                exibir.displayDetailsConsultationRecordDB(ID);
                
                break;
            }

            // Registrar Exame
            case 3: {
                std::string doutor;
                std::cout << "Qual o nome do medico que requisitou?\n";
                std::cin.ignore();
                std::getline(std::cin, doutor);

                std::string nome;
                std::cout << "Qual o nome do exame?\n";
                std::getline(std::cin, nome);

                std::string lab;
                std::cout << "Qual o nome do laboratorio?\n";
                std::getline(std::cin, lab);

                std::string resultado;
                std::cout << "Qual o resultado do exame?\n";
                std::getline(std::cin, resultado);

                std::string data;
                std::cout << "Qual foi a data do exame?\n";
                std::getline(std::cin, data);

                std::string hora;
                while(true){
                    std::cout << "Qual foi o horario do exame? (HH:MM)\n";
                    std::getline(std::cin, hora);
                    Time Verificacao(1,1,1);
                    if(Verificacao.isStringValid(hora)){
                        break;
                    } else {
                        std::cout << "Digite um formato valido! (HH:MM)\n";
                    }
                }

                Time horas(hora);
                ExamRecord exame(*paciente_real, data, horas, nome, resultado, lab, doutor);
                exame.registerDB(ID);
                
                break;
            }

            // Exibir Exames
            case 4: {
                DatabaseMethods exibir;
                exibir.displayDetailsExamRecordDB(ID);

                break;
            }

            // Registrar Glicose
            case 5: {
                std::string data;
                std::cout << "Qual foi a data do teste de glicose?\n";
                std::cin.ignore();
                std::getline(std::cin, data);

                std::string hora;
                while(true){
                    std::cout << "Qual foi o horario do teste de glicose?\n";
                    std::getline(std::cin, hora);
                    Time Verificacao(1,1,1);
                    if(Verificacao.isStringValid(hora)){
                        break;
                    } else {
                        std::cout << "Digite um formato valido! (HH:MM)\n";
                    }
                }

                int glucoselvl;
                std::cout << "Qual foi o nivel de glicose do teste?\n";
                std::cin >> glucoselvl;

                bool jejum;
                char jejumc;
                while (true){
                    std::cout << "Estava de jejum? (S/N)\n";
                    if(std::cin >> jejumc){
                        if(jejumc == 'S'){
                            jejum = true;
                            break;
                        }

                        if (jejumc == 'N'){
                            jejum = false;
                            break;
                        }

                        std::cout << "Digite um caractere valido!\n";

                    } else {
                        std::cout << "Digite um caractere valido!\n";
                    }
                }

                Time horas(hora);
                GlucoseRecord registroGlicose(*paciente_real, data, horas, glucoselvl, jejum);
                registroGlicose.registerDB(ID);

            }

            // Exibir Registros Glicose
            case 6: {
                DatabaseMethods exibir;
                exibir.displayDetailsGlucoseRecordDB(ID);

                break;
            }

            // Registrar Plano Alimentar
            case 7: {
                std::string nutricionista;
                std::cout << "Qual o nome do nutricionista?\n";
                std::cin.ignore();
                std::getline(std::cin, nutricionista);

                std::string alimentos;
                std::cout << "Quais alimentos devem ser evitados?\n";
                std::getline(std::cin, alimentos);

                std::string vitaminas;
                std::cout << "Quais vitaminas sao necessarias?\n";
                std::getline(std::cin, vitaminas);

                int proteinas;
                while(true){
                    std::cout << "Quanto proteina?\n";
                    if(std::cin >> proteinas){
                        break;
                    } else {
                        std::cout << "Digite um valor valido!\n";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    }
                }

                int carboidrato;
                while(true){
                    std::cout << "Quanto de carboidrato?\n";
                    if(std::cin >> carboidrato){
                        break;
                    } else {
                        std::cout << "Digite um valor valido!\n";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    }
                }

                int gordura;
                while(true){
                    std::cout << "Quanto de gordura?\n";
                    if(std::cin >> gordura){
                        break;
                    } else {
                        std::cout << "Digite um valor valido!\n";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    }
                }

                MealPlan plano(alimentos, nutricionista, vitaminas, proteinas, carboidrato, gordura, *paciente_real);
                plano.register_mealPlan(ID);
            }

            case 8: {
                MealPlan plano("-", "-", "-", 0, 0, 0, paciente_temporario);
                plano.change_mealPlan(ID);
            }

            case 10 : {
                continuous = false;
                break;
            }
            
            default : {
                std::cout << "Digite uma opcao valida!\n";
                break;
            }
        }
    }

        
    if (paciente_real != nullptr)
        delete paciente_real;

    return 0;
}
