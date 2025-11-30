#include<iostream>
#include<string>
#include<sqlite3.h>
#include"../include/MealPlan.hpp"

MealPlan::MealPlan(std::string foodAvoided, std::string nutricionist, std::string vitamins,
int protein, int carbohydrate, int fat, const Patient& patient)
: foodAvoided(foodAvoided), nutricionist(nutricionist), vitamins(vitamins), protein(protein),
carbohydrate(carbohydrate), fat(fat), patient(patient){}

MealPlan::~MealPlan(){}

void MealPlan::register_mealPlan(int id){
    //conectando ao banco de  dados
    sqlite3* db;
    sqlite3_stmt* stmt;

    int rc=sqlite3_open("database.db",&db);
    if (rc) {
        std::cerr << "Erro ao abrir banco: " << sqlite3_errmsg(db) << std::endl;
    }
    const char* insert = "INSERT INTO PlanoAlimentar (Paciente, Nutricionista, Vitaminas, Proteinas, Carboidrato, Gordura, AlimentosEvitados) VALUES (?,?,?,?,?,?,?)";
     if (sqlite3_prepare_v2(db, insert, -1, &stmt, nullptr) == SQLITE_OK) {
        //Vincular parâmetros as variaveis
        sqlite3_bind_int(stmt, 1, id);//id referente ao paciente, sera passado na main
        sqlite3_bind_text(stmt, 2, nutricionist.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, vitamins.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 4, protein);
        sqlite3_bind_int(stmt, 5, carbohydrate);
        sqlite3_bind_int(stmt, 6, fat);
        sqlite3_bind_text(stmt, 7, foodAvoided.c_str(), -1, SQLITE_STATIC);
        // Executar inserção
        if (sqlite3_step(stmt) == SQLITE_DONE) {
            std::cout << "Plano alimentar registrado com sucesso com sucesso!" << std::endl;
        } else {
            std::cerr << "Erro ao executar inserção: " << sqlite3_errmsg(db) << std::endl;
        }
        
        // Finalizar statement
        sqlite3_finalize(stmt);
    } 
    else {
        std::cerr << "Erro ao preparar SQL: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_close(db);
}


//funções axiliares ao change
void update_string(int id, const std::string campo, std::string& valor){
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    //atualização no database
    try{
        int rc = sqlite3_open("database.db", &db);
        if (rc != SQLITE_OK) {
            throw std::runtime_error("Erro ao abrir database: " + std::string(sqlite3_errmsg(db)));
        }
        std::string sql = "UPDATE PlanoAlimentar SET " + campo + " = ? WHERE id = ?";
        
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error("Erro ao preparar SQL: " + std::string(sqlite3_errmsg(db)));
        }

        sqlite3_bind_text(stmt, 1, valor.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, id);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            throw std::runtime_error("Erro ao executar UPDATE: " + std::string(sqlite3_errmsg(db)));
        } else {
            std::cout << campo << " atualizado com sucesso!" << std::endl;
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }
    catch (const std::exception& e) {
        std::cerr << "ERRO: " << e.what() << std::endl;
        
        // Limpeza em caso de erro
        if (stmt) sqlite3_finalize(stmt);
        if (db) sqlite3_close(db);
    }
}

void update_int(int id, const std::string campo, int& valor){
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    //atualização no database
    try{
        int rc = sqlite3_open("database.db", &db);
        if (rc != SQLITE_OK) {
            throw std::runtime_error("Erro ao abrir database: " + std::string(sqlite3_errmsg(db)));
        }
        std::string sql = "UPDATE PlanoAlimentar SET " + campo + " = ? WHERE id = ?";
        
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error("Erro ao preparar SQL: " + std::string(sqlite3_errmsg(db)));
        }

        sqlite3_bind_int(stmt, 1, valor);
        sqlite3_bind_int(stmt, 2, id);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            throw std::runtime_error("Erro ao executar UPDATE: " + std::string(sqlite3_errmsg(db)));
        } else {
            std::cout << campo << " atualizado com sucesso!" << std::endl;
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }
    catch (const std::exception& e) {
        std::cerr << "ERRO: " << e.what() << std::endl;
        
        // Limpeza em caso de erro
        if (stmt) sqlite3_finalize(stmt);
        if (db) sqlite3_close(db);
    }
}


void MealPlan::change_mealPlan(int id){
    //conectando ao banco de  dados
    sqlite3* db;
    sqlite3_stmt* stmt;

    int rc=sqlite3_open("database.db",&db);
    if (rc) {
        std::cerr << "Erro ao abrir banco: " << sqlite3_errmsg(db) << std::endl;
    }

    bool continuos = true;
    while(continuos){
        std::cout<<"\nDigite o numero referente ao que deseja alterar:\n";
        int option;
        std::cout<<"\n1 - Nutricionista";
        std::cout<<"\n2 - Vitaminas";
        std::cout<<"\n3 - Proteinas";
        std::cout<<"\n4 - Carboidrato";
        std::cout<<"\n5 - Gorduras";
        std::cout<<"\n6 - Alimentos a evitar";
        std::cout<<"\n7 - Parar alteracoes\n\n";

        if(std::cin>>option){
        } else {
            std::cerr << "Erro ao ler um numero inteiro\n";
            std::cin.clear();
            break;
        }
        std::string valor_string;
        int valor_int;
        switch (option)
        {
        case 1:
           std::cout << "Digite o novo nutricionista: ";
            std::cin.ignore();
            if(std::getline(std::cin, valor_string)){
            update_string(id, "Nutricionista", valor_string);
            nutricionist = valor_string;
            break;
            } else {
                std::cerr << "Erro ao ler STRING nutricionista\n";
                break;
            }
        case 2:
            std::cout << "Digite as vitaminas que quer no seu plano: ";
            std::cin.ignore();
            if(std::getline(std::cin, valor_string)){
            update_string(id, "Vitaminas", valor_string);
            vitamins = valor_string;
            break;
            } else {
                std::cerr << "Erro ao ler STRING vitaminas\n";
                break;
            }
        case 3:
            std::cout << "Digite a quantidade de proteinas que quer no seu plano: ";
            if(std::cin>>valor_int){
            update_int(id, "Proteinas", valor_int);
            protein = valor_int;
            break;
            } else {
                std::cerr << "Erro ao ler INT proteinas\n";
                std::cin.clear();
                break;
            }
        case 4:
            std::cout << "Digite a quantidade de carboidrato que quer no seu plano: ";
            if(std::cin>>valor_int){
            update_int(id, "Carboidrato", valor_int);
            carbohydrate = valor_int;
            break;
            } else {
                std::cerr << "Erro ao ler INT carboidrato\n";
                std::cin.clear();
                break;
            }
        case 5:
            std::cout << "Digite a quantidade de gordura que quer no seu plano: ";
            if(std::cin>>valor_int){
            update_int(id, "Gordura", valor_int);
            fat = valor_int;
            break;
            } else {
                std::cerr << "Erro ao ler INT gordura\n";
                std::cin.clear();
                break;
            }
        case 6:
            std::cout << "Digite os alimentos a serem evitados no seu plano: ";
            std::cin.ignore();
            if(std::getline(std::cin, valor_string)){
            update_string(id, "AlimentosEvitados", valor_string);
            foodAvoided = valor_string;
            break;
            } else {
                std::cerr << "Erro ao ler STRING alimentos evitados\n";
                break;
            }
        default:
            continuos = false;
            break;
        }
    }
    sqlite3_close(db);
}