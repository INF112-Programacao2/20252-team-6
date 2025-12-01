#include <iostream>
#include <iomanip>
#include <string>
#include <sqlite3.h>
#include <regex>
#include "../include/DatabaseMethods.hpp"

DatabaseMethods::DatabaseMethods(){}
DatabaseMethods::~DatabaseMethods(){}

void DatabaseMethods::displayDetailsExamRecordDB(int id) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    
    try {
        int rc = sqlite3_open("database.db", &db);
        if (rc != SQLITE_OK) {
            std::cerr << "Erro ao abrir database: " << sqlite3_errmsg(db) << std::endl;
            return;
        }
        const char* query = 
                "SELECT re.Nome, re.Medico, re.Laboratorio, re.Resultado, rs.Hora, rs.Data "
                "FROM RegistroExame re "
                "JOIN RegistroSaude rs ON re.RegistroSaude = rs.Id "
                "WHERE rs.Paciente = ? "
                "ORDER BY rs.Data DESC, rs.Hora DESC";
            
            if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_int(stmt, 1, id);
                
                //cabeçalho da tabela
                std::cout << "\n" << std::string(150, '-') << "\n";
                std::cout << "Registros de exame do paciente\n";
                std::cout << std::string(150, '-') << "\n";
                
                std::cout << std::left
                        << std::setw(20) << "Exame"
                        << "|" 
                        << std::setw(20) << "Medico"
                        << "|" 
                        << std::setw(20) << "Laboratorio"
                        << "|" 
                        << std::setw(70) << "Resultado"
                        << "|" 
                        << std::setw(10) << "Data"
                        << "|" 
                        << std::setw(5) << "Hora"
                        << "\n";
                std::cout << std::string(150, '-') << "\n";
        
            int recordCount = 0;
                
            //processar cada linha do resultado
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                std::string examName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                std::string doctor = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                std::string lab = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                std::string result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                std::string time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
                std::string date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
                    
                // Limitar o tamanho do resultado para caber na tabela
                if (result.length() > 67) {
                    result = result.substr(0, 67) + "...";
                }

                    std::cout << std::left
                    << std::setw(20) << examName
                        << "|"
                        << std::setw(20) << doctor
                        << "|"
                        << std::setw(20) << lab
                        << "|"
                        << std::setw(70) << result
                        << "|"
                        << std::setw(8) << date
                        << "|"
                        << std::setw(5) << time
                        << "\n";
                    
                    recordCount++;
                }
            std::cout << std::string(150, '-') << "\n";
            std::cout << "Total de registros encontrados: " << recordCount << "\n";
            std::cout << std::string(150, '-') << "\n";
            sqlite3_finalize(stmt);
        }
        else {
            std::cerr << "Erro ao preparar consulta: " << sqlite3_errmsg(db) << std::endl;
        }
        
        sqlite3_close(db);
    }
    catch(const std::exception& e){

    }
}

void DatabaseMethods::displayDetailsConsultationRecordDB(int id){
    sqlite3* db;
    sqlite3_stmt* stmt;
    
    try {
        int rc = sqlite3_open("database.db", &db);
        if (rc != SQLITE_OK) {
            std::cerr << "Erro ao abrir database: " << sqlite3_errmsg(db) << std::endl;
            return;
        }
        const char* query = 
                "SELECT rc.Medico, rc.Especialidade, rc.Local, rc.Descricao, rs.Hora, rs.Data "
                "FROM RegistroConsulta rc "
                "JOIN RegistroSaude rs ON rc.RegistroSaude = rs.Id "
                "WHERE rs.Paciente = ? "
                "ORDER BY rs.Data DESC, rs.Hora DESC";
            
            if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_int(stmt, 1, id);
                
                //cabeçalho da tabela
                std::cout << "\n" << std::string(150, '-') << "\n";
                std::cout << "Registros de consulta do paciente\n";
                std::cout << std::string(150, '-') << "\n";
                
                std::cout << std::left
                        << std::setw(20) << "Medico"
                        << "|" 
                        << std::setw(20) << "Especialidade"
                        << "|" 
                        << std::setw(20) << "Local"
                        << "|" 
                        << std::setw(70) << "Descricao"
                        << "|" 
                        << std::setw(10) << "Data"
                        << "|" 
                        << std::setw(5) << "Hora"
                        << "\n";
                std::cout << std::string(150, '-') << "\n";
        
            int recordCount = 0;
                
            //processar cada linha do resultado
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                std::string doctor = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                std::string specialty = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                std::string local = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                std::string descrition = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                std::string time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
                std::string date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
                    
                // Limitar o tamanho do resultado para caber na tabela
                if (descrition.length() > 67) {
                    descrition = descrition.substr(0, 67) + "...";
                }

                    std::cout << std::left
                    << std::setw(20) << doctor
                        << "|"
                        << std::setw(20) << specialty
                        << "|"
                        << std::setw(20) << local
                        << "|"
                        << std::setw(70) << descrition
                        << "|"
                        << std::setw(8) << date
                        << "|"
                        << std::setw(5) << time
                        << "\n";
                    
                    recordCount++;
                }
            std::cout << std::string(150, '-') << "\n";
            std::cout << "Total de registros encontrados: " << recordCount << "\n";
            std::cout << std::string(150, '-') << "\n";
            sqlite3_finalize(stmt);
        }
        else {
            std::cerr << "Erro ao preparar consulta: " << sqlite3_errmsg(db) << std::endl;
        }
        
        sqlite3_close(db);
    }
    catch(const std::exception& e){

    }
}

void DatabaseMethods::displayDetailsGlucoseRecordDB(int id) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    
    try {
        int rc = sqlite3_open("database.db", &db);
        if (rc != SQLITE_OK) {
            std::cerr << "Erro ao abrir database: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        const char* query = 
            "SELECT rg.NivelGlicose, rg.Jejum, rs.Data, rs.Hora "
            "FROM RegistroGlicose rg "
            "JOIN RegistroSaude rs ON rg.RegistroSaude = rs.Id "
            "WHERE rs.Paciente = ? "
            "ORDER BY rs.Data DESC, rs.Hora DESC";
            
        if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, id);
            
            // Cabeçalho da tabela
            std::cout << "\n" << std::string(38, '-') << "\n";
            std::cout << "Registros de glicose" << "\n";
            std::cout << std::string(38, '-') << "\n";
            
            std::cout << std::left
                      << std::setw(12) << "Glicose"
                      << "|"
                      << std::setw(8) << "Jejum"
                      << "|"
                      << std::setw(10) << "Data"
                      << "|"
                      << std::setw(5) << "Hora"
                      << "\n";
            std::cout << std::string(38, '-') << "\n";
    
            int recordCount = 0;
            
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // NivelGlicose (REAL) - coluna 0
                double glucoseLevel = sqlite3_column_double(stmt, 0);
                
                // Jejum (BLOB) - coluna 1 - TRATAMENTO ESPECIAL PARA BLOB
                bool fasting = false;
                std::string fastingStr = "Nao";
                
                if (sqlite3_column_type(stmt, 1) == SQLITE_BLOB) {
                    const void* blobData = sqlite3_column_blob(stmt, 1);
                    int blobSize = sqlite3_column_bytes(stmt, 1);
                    
                    if (blobData != nullptr && blobSize >= sizeof(bool)) {
                        // Converter BLOB para bool
                        fasting = *static_cast<const bool*>(blobData);
                        fastingStr = fasting ? "Sim" : "Nao";
                    } else {
                        fastingStr = "Erro";
                    }
                } else if (sqlite3_column_type(stmt, 1) == SQLITE_INTEGER) {
                    // Fallback: se for INTEGER em vez de BLOB
                    int fastingInt = sqlite3_column_int(stmt, 1);
                    fastingStr = (fastingInt != 0) ? "Sim" : "Nao";
                } else {
                    fastingStr = "Inválido";
                }
                
                // Data e Hora - colunas 2 e 3
                std::string data = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                std::string hora = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                
                // Exibir na tabela
                std::cout << std::left
                          << std::setw(12) << glucoseLevel
                          << "|"
                          << std::setw(8) << fastingStr
                          << "|"
                          << std::setw(10) << data
                          << "|"
                          << std::setw(5) << hora
                          << "\n";
                
                recordCount++;
            }
            
            std::cout << std::string(38, '-') << "\n";
            std::cout << "Total de registros: " << recordCount << "\n";
            std::cout << std::string(38, '-') << "\n";
            
            sqlite3_finalize(stmt);
        }
        else {
            std::cerr << "Erro ao preparar consulta: " << sqlite3_errmsg(db) << std::endl;
        }
        
        sqlite3_close(db);
    }
    catch(const std::exception& e) {
        std::cerr << "Exceção: " << e.what() << std::endl;
    }
}

void DatabaseMethods::displayDetailsMedicationRecordDB(int id){
   sqlite3* db;
    sqlite3_stmt* stmt;
    
    try {
        int rc = sqlite3_open("database.db", &db);
        if (rc != SQLITE_OK) {
            std::cerr << "Erro ao abrir database: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        const char* query = 
            "SELECT m.Nome, m.Horario, m.Dosagem, m.Medico , rs.Data, rs.Hora "
            "FROM RegistroMedicacao rm "
            "JOIN Medicacao m ON rm.Medicacao = m.Id "
            "JOIN RegistroSaude rs ON rm.RegistroSaude = rs.Id "
            "WHERE rs.Paciente = ? "
            "ORDER BY rs.Data DESC, rs.Hora DESC";
            
        if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, id);
            
            // Cabeçalho da tabela
            std::cout << "\n" << std::string(89, '-') << "\n";
            std::cout << "Registros de Medicacoes" << "\n";
            std::cout << std::string(89, '-') << "\n";
            
            std::cout << std::left
                      << std::setw(20) << "Medicamento"
                      << "|"
                      << std::setw(20) << "Horario"
                      << "|"
                      << std::setw(20) << "Medico"
                      << "|"
                      << std::setw(9) << "Dosagem"
                      << "|"
                      << std::setw(10) << "Data"
                      << "|"
                      << std::setw(5) << "Hora"
                      << "\n";
            std::cout << std::string(89, '-') << "\n";
    
            int recordCount = 0;
            
             while (sqlite3_step(stmt) == SQLITE_ROW) {
                std::string medication = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                std::string hours = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                std::string doctor = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                double dosage = sqlite3_column_double(stmt, 3);
                std::string date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
                std::string time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
                    
                // Exibir na tabela
                std::cout << std::left
                      << std::setw(20) << medication
                      << "|"
                      << std::setw(20) << hours
                      << "|"
                      << std::setw(20) << doctor
                      << "|"
                      << std::setw(7) << dosage << "mg"
                      << "|"
                      << std::setw(8) << date
                      << "|"
                      << std::setw(5) << time
                      << "\n";
                
                recordCount++;
            }
            
            std::cout << std::string(89, '-') << "\n";
            std::cout << "Total de registros: " << recordCount << "\n";
            std::cout << std::string(89, '-') << "\n";
            
            sqlite3_finalize(stmt);
        }
        else {
            std::cerr << "Erro ao preparar consulta: " << sqlite3_errmsg(db) << std::endl;
        }
        
        sqlite3_close(db);
    }
    catch(const std::exception& e) {
        std::cerr << "Exceção: " << e.what() << std::endl;
    }
}

bool isValidName(const std::string& name) {
    std::regex name_regex("^[a-zA-ZÀ-ÿ\\s]+$");
    return std::regex_match(name, name_regex);
}


bool isValidWeight(const std::string& weightStr) {
    std::regex decimal_regex("^[0-9]+(\\.[0-9]+)?$");
    
    if (!std::regex_match(weightStr, decimal_regex)) {
        std::cout << "Peso deve ser um número válido.\n";
        return false;
    }
    
    // Converte e verifica faixa
    double weight = std::stod(weightStr);
    if (weight < 1.0 || weight > 300.0) {
        std::cout << "Peso deve estar entre 1kg e 300kg.\n";
        return false;
    }
    
    return true;
}

bool isValidBloodType(const std::string& bloodType) {
    const std::vector<std::string> validTypes = {
        "A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"
    };

    std::string upper = bloodType;
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);

    return std::find(validTypes.begin(), validTypes.end(), upper) != validTypes.end();
}

bool isValidAge(const std::string& ageStr) {
    std::regex age_regex("^(150|1[0-4][0-9]|[1-9][0-9]?)$");
    return std::regex_match(ageStr, age_regex);
}

bool isValidCPF(std::string& cpfStr) {
    //remove caracteres nao numericos
    std::string cleanCPF;
    for (char c : cpfStr){
        if (std::isdigit(c)) {
            cleanCPF += c;
        }
    }
    
    //verifica se tem 11 digitos
    if (cleanCPF.length() != 11) {
        std::cout << "CPF deve ter 11 dígitos.\n";
        return false;
    }
    cpfStr = cleanCPF;
    return true;
}

bool isValidHeight(const std::string& heightStr) {
    std::regex height_regex("^([0-9]|1\\.[0-9]{1,2}|2\\.[0-9]{1}|3\\.00?)$");
    return std::regex_match(heightStr, height_regex);
}
bool DatabaseMethods::createPatient(){
    sqlite3* db;
    sqlite3_stmt* stmt;
    sqlite3_stmt* stmt2;
    
    try {
        int rc = sqlite3_open("database.db", &db);
        if (rc != SQLITE_OK) {
            std::cerr << "Erro ao abrir database: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        //pedir dados para criar paciente
        std::string name, cpf, password = "1", passwordOK = "0", adress, bloodType, dyabetesType;
        int gender=3;

        std::cout << "\nDigite seu nome: ";
        std::cin.ignore(); 
        std::getline(std::cin, name);

        // Validar o nome
        while (!isValidName(name)) {
            std::cout << "Nome inválido! Use apenas letras e espaços.\n";
            std::cout << "Digite seu nome novamente: ";
            std::getline(std::cin, name);
        }

        //Validar cpf
        std::cout << "\nColeta de dados necessaria para fins do programa!\nDigite seu cpf: ";
        std::cin >> cpf;
        while (!isValidCPF(cpf)) {
            std::cout << "Digite seu CPF novamente: ";
            std::cin >> cpf;
        }

        std::cout << "\nDigite seu endereço: ";
        std::cin.ignore(); 
        std::getline(std::cin, adress);

        //validar idade
        std::string ageInput;
        std::cout << "Digite sua idade: ";
        std::cin >> ageInput;
        while (!isValidAge(ageInput)) {
            std::cout << "Idade inválida! Digite entre 0 e 150 anos (ex: 25.5): ";
            std::cin >> ageInput;
        }
        int age = std::stod(ageInput); // Converte para int

        //validar genero
        while(gender>2||gender<1){
            std::cout << "\nQual seu genero?(1 p/ masculino e 2 p/ feminino):";
            std::cin >> gender;
            if(gender!=1&&gender!=2){
                std::cout<<"\nOpcao invalida, digite novamente";
            }
        }

        //validar tipo sanguineo
        std::cout << "\nDigite seu tipo sanguineo: ";
        std::cin >> bloodType;
        while(!isValidBloodType(bloodType)){
            std::cout << "\nOpcao invalida, digite novamente: ";
            std::cin >> bloodType;
        }

        std::cout << "\nDigite seu tipo de diabetes: ";
        std::cin.ignore(); 
        std::getline(std::cin, dyabetesType);

        //validar peso
        std::string weightInput;
        std::cout << "Digite seu peso em Kg: ";
        std::cin >> weightInput;
        while (!isValidWeight(weightInput)) {
            std::cout << "Peso inválido! Digite entre 1kg e 300kg (ex: 65.5): ";
            std::cin >> weightInput;
        }
        double weight = std::stod(weightInput);

        //validar altura
        std::string heightInput;
        std::cout << "Digite sua altura em metros: ";
        std::cin >> heightInput;
        while (!isValidHeight(heightInput)) {
            std::cout << "Altura inválido! Digite entre 0 e 3 metros (ex: 1.75): ";
            std::cin >> heightInput;
        }
        double height = std::stod(heightInput);


        //validar senha
        while(password!=passwordOK){
            std::cout << "\nDigite sua senha: ";
            std::cin >> password;
            std::cout << "\nConfirme sua senha: ";
            std::cin >> passwordOK;
            if(password!=passwordOK)
                std::cout << "\nSenhas difentes, digite novamente!";
        }

        //transformas genero em string
        std::string genderStr;
        if(gender==1)
            genderStr = "Masculino";
        else
            genderStr = "Feminino";


        //inserir dados no banco
        const char* insert = "INSERT INTO Pessoa (Nome, Idade, Sexo, Cpf, Senha, Endereco) VALUES (?,?,?,?,?,?)";
        if (sqlite3_prepare_v2(db, insert, -1, &stmt, nullptr) == SQLITE_OK) {
            //Vincular parâmetros as variaveis
            sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 2, age);
            sqlite3_bind_text(stmt, 3, genderStr.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 4, cpf.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 5, password.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 6, adress.c_str(), -1, SQLITE_TRANSIENT);

            if (sqlite3_step(stmt) == SQLITE_DONE) {
                std::cout << "Pessoa inserido com sucesso com sucesso!" << std::endl;
                sqlite3_finalize(stmt);
                int idPerson = sqlite3_last_insert_rowid(db);//pega o id da ultima insercao
                const char* insert2 = "INSERT INTO Paciente (Pessoa, Altura, Peso, TipoDiabetes, TipoSanguineo) VALUES (?,?,?,?,?)";
                if (sqlite3_prepare_v2(db, insert2, -1, &stmt2, nullptr) == SQLITE_OK) {
                        sqlite3_bind_int(stmt2, 1, idPerson);
                        sqlite3_bind_double(stmt2, 2, weight);
                        sqlite3_bind_double(stmt2, 3, height);
                        sqlite3_bind_text(stmt2, 4, dyabetesType.c_str(), -1, SQLITE_TRANSIENT);
                        sqlite3_bind_text(stmt2, 5, bloodType.c_str(), -1, SQLITE_TRANSIENT);
                    if (sqlite3_step(stmt2) == SQLITE_DONE) {
                        std::cout << "Paciente registrado com sucesso com sucesso!" << std::endl;
                        sqlite3_finalize(stmt2);
                        return true;
                    } 
                    else {
                        std::cerr << "Erro ao executar registro de paciente: " << sqlite3_errmsg(db) << std::endl;
                        return false;
                    }
                }
                else{
                    std::cerr << "Erro ao preparar SQL: " << sqlite3_errmsg(db) << std::endl;
                    return false;
                }
            } 
            else {
                std::cerr << "Erro ao executar registro de pessoa: " << sqlite3_errmsg(db) << std::endl;
                sqlite3_finalize(stmt);
                return false;
            }
        }
        else {
            std::cerr << "Erro ao preparar SQL: " << sqlite3_errmsg(db) << std::endl;
            return false;
        } 
        sqlite3_close(db);
    }
    catch(const std::exception& e) {
        std::cerr << "Exceção: " << e.what() << std::endl;
        return false;
    }
    return false;
}