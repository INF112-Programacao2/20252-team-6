#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QStatusBar>
#include <QInputDialog>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "../include/Time.hpp"
#include "../include/DatabaseMethods.hpp"
#include "../include/ExamRecord.hpp"
#include "../include/GlucoseRecord.hpp"
#include "../include/MealPlan.hpp"
#include <QTextEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QScrollArea>
#include <sqlite3.h>
#include <iostream>
#include <QRegularExpression>
#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Diarybetes - Sistema de Gerenciamento de Diabetes");
    setMinimumSize(800, 600);

    // Inicializa o DB (cria tabelas se n existir)
    if (!initializeDatabase()) {
        showError("Erro ao inicializar banco de dados. O sistema pode não funcionar corretamente.");
    } else {
        // Mostra onde o DB ta salvo (só no console/debug)
        QString dbPath = getDatabasePath();
        statusBar()->showMessage(QString("Banco de dados: %1").arg(dbPath), 5000);
    }

    statusBar()->showMessage("Sistema iniciado");

    // Botoes desabilitados ate fazer login
    ui->actionsGroupBox->setEnabled(false);

    // Conecta os botoes com os slots
    connect(ui->loginButton, &QPushButton::clicked,
            this, &MainWindow::onLoginClicked);
    connect(ui->createAccountButton, &QPushButton::clicked,
            this, &MainWindow::onCreateAccountClicked);
    connect(ui->btnMarkConsultation, &QPushButton::clicked,
            this, &MainWindow::onMarkConsultation);
    connect(ui->btnViewConsultations, &QPushButton::clicked,
            this, &MainWindow::onViewConsultations);
    connect(ui->btnMarkExam, &QPushButton::clicked,
            this, &MainWindow::onMarkExam);
    connect(ui->btnViewExams, &QPushButton::clicked,
            this, &MainWindow::onViewExams);
    connect(ui->btnRegisterGlucose, &QPushButton::clicked,
            this, &MainWindow::onRegisterGlucose);
    connect(ui->btnViewGlucose, &QPushButton::clicked,
            this, &MainWindow::onViewGlucose);
    connect(ui->btnRegisterMealPlan, &QPushButton::clicked,
            this, &MainWindow::onRegisterMealPlan);
    connect(ui->btnChangeMealPlan, &QPushButton::clicked,
            this, &MainWindow::onChangeMealPlan);
    connect(ui->btnViewMealPlan, &QPushButton::clicked,
            this, &MainWindow::onViewMealPlan);
}

MainWindow::~MainWindow()
{
    // unique_ptr ja libera memoria sozinho, so precisa deletar o ui
    delete ui;
}

void MainWindow::showError(const QString& msg)
{
    QMessageBox::critical(this, "Erro", msg);
    statusBar()->showMessage(msg, 5000);
}

void MainWindow::showInfo(const QString& msg)
{
    QMessageBox::information(this, "Info", msg);
    statusBar()->showMessage(msg, 3000);
}

// Retorna o caminho do DB (onde ta o .exe)
QString MainWindow::getDatabasePath()
{
    // Pega o diretorio do executavel
    QString appDir = QApplication::applicationDirPath();
    QString dbPath = QDir(appDir).absoluteFilePath("database.db");
    
    // Cria o dir se n existir
    QFileInfo dbFile(dbPath);
    QDir dbDir = dbFile.absoluteDir();
    if (!dbDir.exists()) {
        dbDir.mkpath(".");
    }
    
    // Retorna caminho com separadores do sistema
    QString nativePath = QDir::toNativeSeparators(dbPath);
    
    qDebug() << "Caminho do banco de dados:" << nativePath;
    
    return nativePath;
}

// Slot do botao de login
void MainWindow::onLoginClicked()
{
    QString cpf   = ui->cpfLineEdit->text().trimmed();
    const QString senha = ui->senhaLineEdit->text();

    if (cpf.isEmpty() || senha.isEmpty()) {
        showError("CPF e senha não podem ser vazios.");
        return;
    }

    // Remove tudo que n é numero do CPF
    cpf.remove(QRegularExpression("[^0-9]"));
    
    if (cpf.length() != 11) {
        showError("CPF deve ter 11 dígitos.");
        return;
    }

    if (senha.length() < 1) {
        showError("Senha não pode ser vazia.");
        return;
    }

    try {
        // Verifica credenciais
        if (!Patient::verifyLogin(cpf.toStdString(), senha.toStdString())) {
            showError("Credenciais inválidas. Verifique CPF e senha.");
            ui->senhaLineEdit->clear();
            return;
        }

        // Carrega paciente do DB
        Patient* loaded = Patient::loadFromDB(cpf.toStdString());
        if (!loaded) {
            showError("Falha ao carregar dados do paciente a partir do banco.");
            return;
        }

        // unique_ptr ja gerencia memoria, so reseta
        m_currentPatient.reset(loaded);

        // Pega o ID do paciente no DB
        m_patientId = m_currentPatient->searchId();
        if (m_patientId <= 0) {
            showError("Não foi possível obter o ID do paciente no banco.");
            return;
        }

        // Atualiza UI depois do login
        ui->loginGroupBox->setVisible(false);
        ui->actionsGroupBox->setEnabled(true);
        ui->titleLabel->setText(QString("Bem-vindo, %1").arg(QString::fromStdString(m_currentPatient->getName())));
        ui->subtitleLabel->setText(QString("CPF: %1").arg(cpf));
        
        QString msg = QString("Login realizado com sucesso! Bem-vindo, %1.")
                          .arg(QString::fromStdString(m_currentPatient->getName()));
        showInfo(msg);
        
        // Limpa os campos
        ui->cpfLineEdit->clear();
        ui->senhaLineEdit->clear();
        
    } catch (const std::invalid_argument& e) {
        showError(QString("Dados inválidos: %1").arg(e.what()));
        ui->senhaLineEdit->clear();
    } catch (const std::runtime_error& e) {
        showError(QString("Erro ao tentar login: %1").arg(e.what()));
        ui->senhaLineEdit->clear();
    } catch (const std::exception& e) {
        showError(QString("Erro ao tentar login: %1").arg(e.what()));
        ui->senhaLineEdit->clear();
    } catch (...) {
        showError("Erro desconhecido ao tentar login. Tente novamente.");
        ui->senhaLineEdit->clear();
    }
}

// Marcar consulta
void MainWindow::onMarkConsultation()
{
    if (!m_currentPatient || m_patientId <= 0) {
        showError("Nenhum paciente logado. Faça login primeiro.");
        return;
    }

    // Dialog pra pegar os dados da consulta
    QDialog dialog(this);
    dialog.setWindowTitle("Marcar consulta");

    QFormLayout* form = new QFormLayout(&dialog);

    QLineEdit* localEdit = new QLineEdit(&dialog);
    QLineEdit* areaEdit = new QLineEdit(&dialog);
    QLineEdit* medicoEdit = new QLineEdit(&dialog);
    QLineEdit* dataEdit = new QLineEdit(&dialog);
    QLineEdit* horaEdit = new QLineEdit(&dialog);
    QLineEdit* descricaoEdit = new QLineEdit(&dialog);

    dataEdit->setPlaceholderText("DD/MM/AAAA");
    horaEdit->setPlaceholderText("HH:MM ou HH:MM:SS");

    form->addRow("Local:", localEdit);
    form->addRow("Especialidade:", areaEdit);
    form->addRow("Médico:", medicoEdit);
    form->addRow("Data:", dataEdit);
    form->addRow("Hora:", horaEdit);
    form->addRow("Descrição:", descricaoEdit);

    QDialogButtonBox* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal,
        &dialog
    );
    form->addRow(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return; // usuario cancelou
    }

    const QString local      = localEdit->text().trimmed();
    const QString area       = areaEdit->text().trimmed();
    const QString medico     = medicoEdit->text().trimmed();
    const QString data       = dataEdit->text().trimmed();
    const QString hora       = horaEdit->text().trimmed();
    const QString descricao  = descricaoEdit->text().trimmed();

    if (local.isEmpty() || area.isEmpty() || medico.isEmpty() ||
        data.isEmpty() || hora.isEmpty() || descricao.isEmpty()) {
        showError("Todos os campos devem ser preenchidos.");
        return;
    }

    try {
        // Valida hora antes de criar
        Time timeObj(hora.toStdString());
        
        // Usa o metodo do Patient pra marcar consulta
        m_currentPatient->makeAppointment(
            data.toStdString(),
            hora.toStdString(),
            medico.toStdString(),
            area.toStdString(),
            descricao.toStdString(),
            local.toStdString()
        );

        showInfo("Consulta marcada com sucesso.");
    } catch (const std::invalid_argument& e) {
        showError(QString("Dados inválidos: %1").arg(e.what()));
    } catch (const std::runtime_error& e) {
        showError(QString("Erro ao marcar consulta: %1").arg(e.what()));
    } catch (const std::exception& e) {
        showError(QString("Erro ao marcar consulta: %1").arg(e.what()));
    } catch (...) {
        showError("Erro desconhecido ao marcar consulta. Verifique os dados e tente novamente.");
    }
}

// Cria novo paciente no DB
void MainWindow::onCreateAccountClicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Criar novo paciente");

    QFormLayout* form = new QFormLayout(&dialog);

    QLineEdit* nameEdit     = new QLineEdit(&dialog);
    QLineEdit* cpfEdit      = new QLineEdit(&dialog);
    QLineEdit* addressEdit  = new QLineEdit(&dialog);
    QLineEdit* genderEdit   = new QLineEdit(&dialog);
    QLineEdit* ageEdit      = new QLineEdit(&dialog);
    QLineEdit* passwordEdit = new QLineEdit(&dialog);
    QLineEdit* diabetesEdit = new QLineEdit(&dialog);
    QLineEdit* bloodEdit    = new QLineEdit(&dialog);
    QLineEdit* weightEdit   = new QLineEdit(&dialog);
    QLineEdit* heightEdit   = new QLineEdit(&dialog);

    passwordEdit->setEchoMode(QLineEdit::Password);
    bloodEdit->setPlaceholderText("A+, A-, B+, B-, AB+, AB-, O+, O-");

    form->addRow("Nome:",          nameEdit);
    form->addRow("CPF:",           cpfEdit);
    form->addRow("Endereco:",      addressEdit);
    form->addRow("Genero:",        genderEdit);
    form->addRow("Idade:",         ageEdit);
    form->addRow("Senha:",         passwordEdit);
    form->addRow("Tipo Diabetes:", diabetesEdit);
    form->addRow("Tipo Sanguineo:", bloodEdit);
    form->addRow("Peso (kg):",     weightEdit);
    form->addRow("Altura (m):",    heightEdit);

    QDialogButtonBox* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal,
        &dialog
    );
    form->addRow(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return; // usuario cancelou
    }

    // Pega e valida os dados
    QString name     = nameEdit->text().trimmed();
    QString cpf      = cpfEdit->text().trimmed();
    QString address  = addressEdit->text().trimmed();
    QString gender   = genderEdit->text().trimmed();
    QString ageStr   = ageEdit->text().trimmed();
    QString pass     = passwordEdit->text();
    QString diab     = diabetesEdit->text().trimmed();
    QString blood    = bloodEdit->text().trimmed().toUpper();
    QString weightStr= weightEdit->text().trimmed();
    QString heightStr= heightEdit->text().trimmed();

    if (name.isEmpty() || cpf.isEmpty() || address.isEmpty() ||
        gender.isEmpty() || ageStr.isEmpty() || pass.isEmpty() ||
        diab.isEmpty() || blood.isEmpty() || weightStr.isEmpty() ||
        heightStr.isEmpty()) {
        showError("Todos os campos são obrigatórios.");
        return;
    }

    bool okAge = false, okWeight = false, okHeight = false;
    int    age    = ageStr.toInt(&okAge);
    double weight = weightStr.toDouble(&okWeight);
    double height = heightStr.toDouble(&okHeight);

    if (!okAge || !okWeight || !okHeight) {
        showError("Idade, peso e altura devem ser numéricos.");
        return;
    }

    // Validacoes extras
    if (age < 0 || age > 150) {
        showError("Idade deve estar entre 0 e 150 anos.");
        return;
    }

    if (weight <= 0 || weight > 500) {
        showError("Peso deve estar entre 0.1kg e 500kg.");
        return;
    }

    if (height <= 0 || height > 3.0) {
        showError("Altura deve estar entre 0.1m e 3.0m.");
        return;
    }

    if (pass.length() < 3) {
        showError("Senha deve ter pelo menos 3 caracteres.");
        return;
    }

    // Remove tudo que n é numero do CPF
    QString cleanCPF = cpf;
    cleanCPF.remove(QRegularExpression("[^0-9]"));
    
    if (cleanCPF.length() != 11) {
        showError("CPF deve ter 11 dígitos.");
        return;
    }

    try {
        // Verifica se CPF ja existe no DB
        DatabaseMethods dbMethods;
        std::string cpfStr = cleanCPF.toStdString();
        if (!dbMethods.isValidCPF(cpfStr)) {
            showError("CPF já cadastrado no sistema ou inválido. Verifique o CPF digitado.");
            return;
        }

        // Valida tipo sanguineo
        if (!Patient::isValidBloodType(blood.toStdString())) {
            showError("Tipo sanguíneo inválido. Use A+, A-, B+, B-, AB+, AB-, O+, O-.");
            return;
        }

        // Cria e salva o paciente
        Patient novo(
            name.toStdString(),
            cleanCPF.toStdString(),
            address.toStdString(),
            gender.toStdString(),
            age,
            pass.toStdString(),
            diab.toStdString(),
            blood.toStdString(),
            weight,
            height
        );

        novo.saveToDB();

        showInfo("Paciente criado e salvo no banco com sucesso. Você já pode fazer login!");

    } catch (const std::invalid_argument& e) {
        showError(QString("Dados inválidos: %1").arg(e.what()));
    } catch (const std::runtime_error& e) {
        showError(QString("Erro ao criar paciente: %1").arg(e.what()));
    } catch (const std::exception& e) {
        showError(QString("Erro ao criar paciente: %1").arg(e.what()));
    } catch (...) {
        showError("Erro desconhecido ao criar paciente. Verifique os dados e tente novamente.");
    }
}

// Exibir consultas marcadas
void MainWindow::onViewConsultations()
{
    if (!m_currentPatient || m_patientId <= 0) {
        showError("Nenhum paciente logado. Faça login primeiro.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Consultas Marcadas");
    dialog.resize(900, 500);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    QTextEdit* textEdit = new QTextEdit(&dialog);
    textEdit->setReadOnly(true);
    textEdit->setFont(QFont("Courier", 9));
    layout->addWidget(textEdit);

    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok, Qt::Horizontal, &dialog);
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);

    // Busca direto do DB e mostra
    sqlite3* database = nullptr;
    sqlite3_stmt* stmt = nullptr;
    
    int recordCount = 0;
    
    try {
        QString output;
        
        // Abre o DB
        QString dbPath = getDatabasePath();
        int rc = sqlite3_open(dbPath.toUtf8().constData(), &database);
        if (rc != SQLITE_OK) {
            showError(QString("Erro ao abrir banco de dados: %1").arg(sqlite3_errmsg(database)));
            if (database) sqlite3_close(database);
            return;
        }
        
        // Query pra pegar as consultas
        const char* query = 
            "SELECT rc.Medico, rc.Especialidade, rc.Local, rc.Descricao, rs.Hora, rs.Data "
            "FROM RegistroConsulta rc "
            "JOIN RegistroSaude rs ON rc.RegistroSaude = rs.Id "
            "WHERE rs.Paciente = ? "
            "ORDER BY rs.Data DESC, rs.Hora DESC";
        
        if (sqlite3_prepare_v2(database, query, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, m_patientId);
            
            output += "Registros de consulta do paciente\n";
            output += QString(150, '-') + "\n";
            output += QString("%1|%2|%3|%4|%5|%6\n")
                .arg("Médico", -20)
                .arg("Especialidade", -20)
                .arg("Local", -20)
                .arg("Descrição", -70)
                .arg("Data", -10)
                .arg("Hora", -5);
            output += QString(150, '-') + "\n";
            
            recordCount = 0;
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                QString doctor = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
                QString specialty = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
                QString local = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
                QString descrition = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
                QString time = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
                QString date = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
                
                if (descrition.length() > 67) {
                    descrition = descrition.left(67) + "...";
                }
                
                output += QString("%1|%2|%3|%4|%5|%6\n")
                    .arg(doctor, -20)
                    .arg(specialty, -20)
                    .arg(local, -20)
                    .arg(descrition, -70)
                    .arg(date, -10)
                    .arg(time, -5);
                
                recordCount++;
            }
            
            output += QString(150, '-') + "\n";
            output += QString("Total de registros encontrados: %1\n").arg(recordCount);
            output += QString(150, '-') + "\n";
            
            sqlite3_finalize(stmt);
            stmt = nullptr;
        } else {
            showError(QString("Erro ao preparar consulta: %1").arg(sqlite3_errmsg(database)));
            if (database) sqlite3_close(database);
            return;
        }
        
        sqlite3_close(database);
        database = nullptr;
        textEdit->setPlainText(output);
        
        if (recordCount == 0) {
            showInfo("Nenhuma consulta encontrada.");
        }
        
    } catch (const std::exception& e) {
        showError(QString("Erro ao buscar consultas: %1").arg(e.what()));
        if (stmt) sqlite3_finalize(stmt);
        if (database) sqlite3_close(database);
        return;
    } catch (...) {
        showError("Erro desconhecido ao buscar consultas.");
        if (stmt) sqlite3_finalize(stmt);
        if (database) sqlite3_close(database);
        return;
    }
    
    dialog.exec();
}

// Marcar exame
void MainWindow::onMarkExam()
{
    if (!m_currentPatient || m_patientId <= 0) {
        showError("Nenhum paciente logado. Faça login primeiro.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Marcar exame");

    QFormLayout* form = new QFormLayout(&dialog);

    QLineEdit* doutorEdit = new QLineEdit(&dialog);
    QLineEdit* nomeEdit = new QLineEdit(&dialog);
    QLineEdit* labEdit = new QLineEdit(&dialog);
    QLineEdit* resultadoEdit = new QLineEdit(&dialog);
    QLineEdit* dataEdit = new QLineEdit(&dialog);
    QLineEdit* horaEdit = new QLineEdit(&dialog);

    dataEdit->setPlaceholderText("DD/MM/AAAA");
    horaEdit->setPlaceholderText("HH:MM ou HH:MM:SS");

    form->addRow("Médico que requisitou:", doutorEdit);
    form->addRow("Nome do exame:", nomeEdit);
    form->addRow("Laboratório:", labEdit);
    form->addRow("Resultado:", resultadoEdit);
    form->addRow("Data:", dataEdit);
    form->addRow("Hora:", horaEdit);

    QDialogButtonBox* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal,
        &dialog
    );
    form->addRow(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    const QString doutor = doutorEdit->text().trimmed();
    const QString nome = nomeEdit->text().trimmed();
    const QString lab = labEdit->text().trimmed();
    const QString resultado = resultadoEdit->text().trimmed();
    const QString data = dataEdit->text().trimmed();
    const QString hora = horaEdit->text().trimmed();

    if (doutor.isEmpty() || nome.isEmpty() || lab.isEmpty() || 
        resultado.isEmpty() || data.isEmpty() || hora.isEmpty()) {
        showError("Todos os campos devem ser preenchidos.");
        return;
    }

    try {
        // Valida hora
        Time timeObj(hora.toStdString());

        m_currentPatient->bookExam(
            data.toStdString(),
            hora.toStdString(),
            nome.toStdString(),
            doutor.toStdString(),
            lab.toStdString(),
            resultado.toStdString()
        );

        showInfo("Exame marcado com sucesso.");
    } catch (const std::invalid_argument& e) {
        showError(QString("Dados inválidos: %1").arg(e.what()));
    } catch (const std::runtime_error& e) {
        showError(QString("Erro ao marcar exame: %1").arg(e.what()));
    } catch (const std::exception& e) {
        showError(QString("Erro ao marcar exame: %1").arg(e.what()));
    } catch (...) {
        showError("Erro desconhecido ao marcar exame. Verifique os dados e tente novamente.");
    }
}

// Exibir exames marcados
void MainWindow::onViewExams()
{
    if (!m_currentPatient || m_patientId <= 0) {
        showError("Nenhum paciente logado. Faça login primeiro.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Exames Marcados");
    dialog.resize(900, 500);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    QTextEdit* textEdit = new QTextEdit(&dialog);
    textEdit->setReadOnly(true);
    textEdit->setFont(QFont("Courier", 9));
    layout->addWidget(textEdit);

    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok, Qt::Horizontal, &dialog);
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);

    sqlite3* database = nullptr;
    sqlite3_stmt* stmt = nullptr;
    
    int recordCount = 0;
    
    try {
        QString output;
        
        QString dbPath = getDatabasePath();
        int rc = sqlite3_open(dbPath.toUtf8().constData(), &database);
        if (rc != SQLITE_OK) {
            showError(QString("Erro ao abrir banco de dados: %1").arg(sqlite3_errmsg(database)));
            if (database) sqlite3_close(database);
            return;
        }
        
        const char* query = 
            "SELECT re.Nome, re.Medico, re.Laboratorio, re.Resultado, rs.Hora, rs.Data "
            "FROM RegistroExame re "
            "JOIN RegistroSaude rs ON re.RegistroSaude = rs.Id "
            "WHERE rs.Paciente = ? "
            "ORDER BY rs.Data DESC, rs.Hora DESC";
        
        if (sqlite3_prepare_v2(database, query, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, m_patientId);
            
            output += "Registros de exame do paciente\n";
            output += QString(150, '-') + "\n";
            output += QString("%1|%2|%3|%4|%5|%6\n")
                .arg("Exame", -20)
                .arg("Médico", -20)
                .arg("Laboratório", -20)
                .arg("Resultado", -70)
                .arg("Data", -10)
                .arg("Hora", -5);
            output += QString(150, '-') + "\n";
            
            recordCount = 0;
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                QString examName = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
                QString doctor = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
                QString lab = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
                QString result = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
                QString time = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
                QString date = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
                
                if (result.length() > 67) {
                    result = result.left(67) + "...";
                }
                
                output += QString("%1|%2|%3|%4|%5|%6\n")
                    .arg(examName, -20)
                    .arg(doctor, -20)
                    .arg(lab, -20)
                    .arg(result, -70)
                    .arg(date, -10)
                    .arg(time, -5);
                
                recordCount++;
            }
            
            output += QString(150, '-') + "\n";
            output += QString("Total de registros encontrados: %1\n").arg(recordCount);
            output += QString(150, '-') + "\n";
            
            sqlite3_finalize(stmt);
            stmt = nullptr;
        } else {
            showError(QString("Erro ao preparar consulta: %1").arg(sqlite3_errmsg(database)));
            if (database) sqlite3_close(database);
            return;
        }
        
        sqlite3_close(database);
        database = nullptr;
        textEdit->setPlainText(output);
        
        if (recordCount == 0) {
            showInfo("Nenhum exame encontrado.");
        }
        
    } catch (const std::exception& e) {
        showError(QString("Erro ao buscar exames: %1").arg(e.what()));
        if (stmt) sqlite3_finalize(stmt);
        if (database) sqlite3_close(database);
        return;
    } catch (...) {
        showError("Erro desconhecido ao buscar exames.");
        if (stmt) sqlite3_finalize(stmt);
        if (database) sqlite3_close(database);
        return;
    }
    
    dialog.exec();
}

// Registrar nivel de glicose
void MainWindow::onRegisterGlucose()
{
    if (!m_currentPatient || m_patientId <= 0) {
        showError("Nenhum paciente logado. Faça login primeiro.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Registrar nível de glicose");

    QFormLayout* form = new QFormLayout(&dialog);

    QLineEdit* dataEdit = new QLineEdit(&dialog);
    QLineEdit* horaEdit = new QLineEdit(&dialog);
    QSpinBox* glicoseEdit = new QSpinBox(&dialog);
    QCheckBox* jejumCheck = new QCheckBox("Estava em jejum?", &dialog);

    dataEdit->setPlaceholderText("DD/MM/AAAA");
    horaEdit->setPlaceholderText("HH:MM ou HH:MM:SS");
    glicoseEdit->setRange(0, 1000);
    glicoseEdit->setValue(100);

    form->addRow("Data:", dataEdit);
    form->addRow("Hora:", horaEdit);
    form->addRow("Nível de glicose:", glicoseEdit);
    form->addRow(jejumCheck);

    QDialogButtonBox* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal,
        &dialog
    );
    form->addRow(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    const QString data = dataEdit->text().trimmed();
    const QString hora = horaEdit->text().trimmed();
    int glicose = glicoseEdit->value();
    bool jejum = jejumCheck->isChecked();

    if (data.isEmpty() || hora.isEmpty()) {
        showError("Data e hora devem ser preenchidos.");
        return;
    }

    try {
        // Valida hora
        Time timeObj(hora.toStdString());

        GlucoseRecord registroGlicose(*m_currentPatient, data.toStdString(), timeObj, glicose, jejum);
        registroGlicose.registerDB(m_patientId);

        showInfo("Registro de glicose salvo com sucesso.");
    } catch (const std::invalid_argument& e) {
        showError(QString("Dados inválidos: %1").arg(e.what()));
    } catch (const std::runtime_error& e) {
        showError(QString("Erro ao registrar glicose: %1").arg(e.what()));
    } catch (const std::exception& e) {
        showError(QString("Erro ao registrar glicose: %1").arg(e.what()));
    } catch (...) {
        showError("Erro desconhecido ao registrar glicose. Verifique os dados e tente novamente.");
    }
}

// Exibir registros de glicose
void MainWindow::onViewGlucose()
{
    if (!m_currentPatient || m_patientId <= 0) {
        showError("Nenhum paciente logado. Faça login primeiro.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Registros de Glicose");
    dialog.resize(600, 400);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    QTextEdit* textEdit = new QTextEdit(&dialog);
    textEdit->setReadOnly(true);
    textEdit->setFont(QFont("Courier", 9));
    layout->addWidget(textEdit);

    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok, Qt::Horizontal, &dialog);
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);

    sqlite3* database = nullptr;
    sqlite3_stmt* stmt = nullptr;
    
    int recordCount = 0;
    
    try {
        QString output;
        
        QString dbPath = getDatabasePath();
        int rc = sqlite3_open(dbPath.toUtf8().constData(), &database);
        if (rc != SQLITE_OK) {
            showError(QString("Erro ao abrir banco de dados: %1").arg(sqlite3_errmsg(database)));
            if (database) sqlite3_close(database);
            return;
        }
        
        const char* query = 
            "SELECT rg.NivelGlicose, rg.Jejum, rs.Data, rs.Hora "
            "FROM RegistroGlicose rg "
            "JOIN RegistroSaude rs ON rg.RegistroSaude = rs.Id "
            "WHERE rs.Paciente = ? "
            "ORDER BY rs.Data DESC, rs.Hora DESC";
        
        if (sqlite3_prepare_v2(database, query, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, m_patientId);
            
            output += "Registros de glicose\n";
            output += QString(38, '-') + "\n";
            output += QString("%1|%2|%3|%4\n")
                .arg("Glicose", -12)
                .arg("Jejum", -8)
                .arg("Data", -10)
                .arg("Hora", -5);
            output += QString(38, '-') + "\n";
            
            recordCount = 0;
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                double glucoseLevel = sqlite3_column_double(stmt, 0);
                QString fastingStr = "Não";
                
                if (sqlite3_column_type(stmt, 1) == SQLITE_BLOB) {
                    const void* blobData = sqlite3_column_blob(stmt, 1);
                    int blobSize = sqlite3_column_bytes(stmt, 1);
                    if (blobData != nullptr && blobSize >= static_cast<int>(sizeof(bool))) {
                        bool fasting = *static_cast<const bool*>(blobData);
                        fastingStr = fasting ? "Sim" : "Não";
                    }
                } else if (sqlite3_column_type(stmt, 1) == SQLITE_INTEGER) {
                    int fastingInt = sqlite3_column_int(stmt, 1);
                    fastingStr = (fastingInt != 0) ? "Sim" : "Não";
                }
                
                QString date = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
                QString time = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
                
                output += QString("%1|%2|%3|%4\n")
                    .arg(QString::number(glucoseLevel), -12)
                    .arg(fastingStr, -8)
                    .arg(date, -10)
                    .arg(time, -5);
                
                recordCount++;
            }
            
            output += QString(38, '-') + "\n";
            output += QString("Total de registros: %1\n").arg(recordCount);
            output += QString(38, '-') + "\n";
            
            sqlite3_finalize(stmt);
            stmt = nullptr;
        } else {
            showError(QString("Erro ao preparar consulta: %1").arg(sqlite3_errmsg(database)));
            if (database) sqlite3_close(database);
            return;
        }
        
        sqlite3_close(database);
        database = nullptr;
        textEdit->setPlainText(output);
        
        if (recordCount == 0) {
            showInfo("Nenhum registro de glicose encontrado.");
        }
        
    } catch (const std::exception& e) {
        showError(QString("Erro ao buscar registros de glicose: %1").arg(e.what()));
        if (stmt) sqlite3_finalize(stmt);
        if (database) sqlite3_close(database);
        return;
    } catch (...) {
        showError("Erro desconhecido ao buscar registros de glicose.");
        if (stmt) sqlite3_finalize(stmt);
        if (database) sqlite3_close(database);
        return;
    }
    
    dialog.exec();
}

// Registrar plano alimentar
void MainWindow::onRegisterMealPlan()
{
    if (!m_currentPatient || m_patientId <= 0) {
        showError("Nenhum paciente logado. Faça login primeiro.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Registrar plano alimentar");

    QFormLayout* form = new QFormLayout(&dialog);

    QLineEdit* nutricionistaEdit = new QLineEdit(&dialog);
    QLineEdit* alimentosEdit = new QLineEdit(&dialog);
    QLineEdit* vitaminasEdit = new QLineEdit(&dialog);
    QSpinBox* proteinasEdit = new QSpinBox(&dialog);
    QSpinBox* carboidratoEdit = new QSpinBox(&dialog);
    QSpinBox* gorduraEdit = new QSpinBox(&dialog);

    proteinasEdit->setRange(0, 10000);
    carboidratoEdit->setRange(0, 10000);
    gorduraEdit->setRange(0, 10000);

    form->addRow("Nutricionista:", nutricionistaEdit);
    form->addRow("Alimentos a evitar:", alimentosEdit);
    form->addRow("Vitaminas necessárias:", vitaminasEdit);
    form->addRow("Proteínas (g):", proteinasEdit);
    form->addRow("Carboidratos (g):", carboidratoEdit);
    form->addRow("Gorduras (g):", gorduraEdit);

    QDialogButtonBox* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal,
        &dialog
    );
    form->addRow(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    const QString nutricionista = nutricionistaEdit->text().trimmed();
    const QString alimentos = alimentosEdit->text().trimmed();
    const QString vitaminas = vitaminasEdit->text().trimmed();
    int proteinas = proteinasEdit->value();
    int carboidrato = carboidratoEdit->value();
    int gordura = gorduraEdit->value();

    if (nutricionista.isEmpty() || alimentos.isEmpty() || vitaminas.isEmpty()) {
        showError("Todos os campos de texto devem ser preenchidos.");
        return;
    }

    try {
        MealPlan plano(
            alimentos.toStdString(),
            nutricionista.toStdString(),
            vitaminas.toStdString(),
            proteinas,
            carboidrato,
            gordura,
            *m_currentPatient
        );
        plano.register_mealPlan(m_patientId);

        showInfo("Plano alimentar registrado com sucesso.");
    } catch (const std::invalid_argument& e) {
        showError(QString("Dados inválidos: %1").arg(e.what()));
    } catch (const std::runtime_error& e) {
        showError(QString("Erro ao registrar plano alimentar: %1").arg(e.what()));
    } catch (const std::exception& e) {
        showError(QString("Erro ao registrar plano alimentar: %1").arg(e.what()));
    } catch (...) {
        showError("Erro desconhecido ao registrar plano alimentar. Verifique os dados e tente novamente.");
    }
}

// Alterar plano alimentar
void MainWindow::onChangeMealPlan()
{
    if (!m_currentPatient || m_patientId <= 0) {
        showError("Nenhum paciente logado. Faça login primeiro.");
        return;
    }

    // Pega o ID do plano alimentar do paciente
    sqlite3* database = nullptr;
    sqlite3_stmt* stmt = nullptr;
    int planoId = -1;

    QString dbPath = getDatabasePath();
    int rc = sqlite3_open(dbPath.toUtf8().constData(), &database);
    if (rc != SQLITE_OK) {
        showError(QString("Erro ao abrir banco de dados: %1").arg(sqlite3_errmsg(database)));
        if (database) sqlite3_close(database);
        return;
    }

    const char* query = "SELECT Id FROM PlanoAlimentar WHERE Paciente = ?";
    if (sqlite3_prepare_v2(database, query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, m_patientId);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            planoId = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
    } else {
        showError(QString("Erro ao preparar consulta: %1").arg(sqlite3_errmsg(database)));
        sqlite3_close(database);
        return;
    }
    sqlite3_close(database);
    database = nullptr;

    if (planoId == -1) {
        showError("Nenhum plano alimentar encontrado. Registre um plano primeiro.");
        return;
    }

    // Dialog pra alterar o plano
    bool continuar = true;
    while (continuar) {
        QDialog dialog(this);
        dialog.setWindowTitle("Alterar plano alimentar");

        QVBoxLayout* layout = new QVBoxLayout(&dialog);
        QLabel* label = new QLabel("O que deseja alterar?", &dialog);
        layout->addWidget(label);

        QPushButton* btnNutricionista = new QPushButton("1 - Nutricionista", &dialog);
        QPushButton* btnVitaminas = new QPushButton("2 - Vitaminas", &dialog);
        QPushButton* btnProteinas = new QPushButton("3 - Proteínas", &dialog);
        QPushButton* btnCarboidrato = new QPushButton("4 - Carboidrato", &dialog);
        QPushButton* btnGordura = new QPushButton("5 - Gorduras", &dialog);
        QPushButton* btnAlimentos = new QPushButton("6 - Alimentos a evitar", &dialog);
        QPushButton* btnSair = new QPushButton("7 - Sair", &dialog);

        layout->addWidget(btnNutricionista);
        layout->addWidget(btnVitaminas);
        layout->addWidget(btnProteinas);
        layout->addWidget(btnCarboidrato);
        layout->addWidget(btnGordura);
        layout->addWidget(btnAlimentos);
        layout->addWidget(btnSair);

        int option = 0;
        connect(btnNutricionista, &QPushButton::clicked, [&option, &dialog]() { option = 1; dialog.accept(); });
        connect(btnVitaminas, &QPushButton::clicked, [&option, &dialog]() { option = 2; dialog.accept(); });
        connect(btnProteinas, &QPushButton::clicked, [&option, &dialog]() { option = 3; dialog.accept(); });
        connect(btnCarboidrato, &QPushButton::clicked, [&option, &dialog]() { option = 4; dialog.accept(); });
        connect(btnGordura, &QPushButton::clicked, [&option, &dialog]() { option = 5; dialog.accept(); });
        connect(btnAlimentos, &QPushButton::clicked, [&option, &dialog]() { option = 6; dialog.accept(); });
        connect(btnSair, &QPushButton::clicked, [&continuar, &dialog]() { continuar = false; dialog.reject(); });

        if (dialog.exec() != QDialog::Accepted) {
            break;
        }

        sqlite3* db = nullptr;
        sqlite3_stmt* updateStmt = nullptr;
        
        try {
            QString dbPath = getDatabasePath();
            rc = sqlite3_open(dbPath.toUtf8().constData(), &db);
            if (rc != SQLITE_OK) {
                showError(QString("Erro ao abrir banco de dados: %1").arg(sqlite3_errmsg(db)));
                if (db) sqlite3_close(db);
                continue;
            }

            switch (option) {
                case 1: {
                    bool ok;
                    QString valor = QInputDialog::getText(this, "Alterar Nutricionista", "Digite o novo nutricionista:", QLineEdit::Normal, "", &ok);
                    if (ok && !valor.isEmpty()) {
                        std::string sql = "UPDATE PlanoAlimentar SET Nutricionista = ? WHERE Id = ?";
                        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &updateStmt, nullptr) == SQLITE_OK) {
                            sqlite3_bind_text(updateStmt, 1, valor.toStdString().c_str(), -1, SQLITE_TRANSIENT);
                            sqlite3_bind_int(updateStmt, 2, planoId);
                            if (sqlite3_step(updateStmt) == SQLITE_DONE) {
                                showInfo("Nutricionista atualizado com sucesso!");
                            } else {
                                showError(QString("Erro ao atualizar: %1").arg(sqlite3_errmsg(db)));
                            }
                            sqlite3_finalize(updateStmt);
                            updateStmt = nullptr;
                        } else {
                            showError(QString("Erro ao preparar atualização: %1").arg(sqlite3_errmsg(db)));
                        }
                    }
                    break;
                }
                case 2: {
                    bool ok;
                    QString valor = QInputDialog::getText(this, "Alterar Vitaminas", "Digite as vitaminas:", QLineEdit::Normal, "", &ok);
                    if (ok && !valor.isEmpty()) {
                        std::string sql = "UPDATE PlanoAlimentar SET Vitaminas = ? WHERE Id = ?";
                        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &updateStmt, nullptr) == SQLITE_OK) {
                            sqlite3_bind_text(updateStmt, 1, valor.toStdString().c_str(), -1, SQLITE_TRANSIENT);
                            sqlite3_bind_int(updateStmt, 2, planoId);
                            if (sqlite3_step(updateStmt) == SQLITE_DONE) {
                                showInfo("Vitaminas atualizadas com sucesso!");
                            } else {
                                showError(QString("Erro ao atualizar: %1").arg(sqlite3_errmsg(db)));
                            }
                            sqlite3_finalize(updateStmt);
                            updateStmt = nullptr;
                        } else {
                            showError(QString("Erro ao preparar atualização: %1").arg(sqlite3_errmsg(db)));
                        }
                    }
                    break;
                }
                case 3: {
                    bool ok;
                    int valor = QInputDialog::getInt(this, "Alterar Proteínas", "Digite a quantidade de proteínas:", 0, 0, 10000, 1, &ok);
                    if (ok) {
                        std::string sql = "UPDATE PlanoAlimentar SET Proteinas = ? WHERE Id = ?";
                        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &updateStmt, nullptr) == SQLITE_OK) {
                            sqlite3_bind_int(updateStmt, 1, valor);
                            sqlite3_bind_int(updateStmt, 2, planoId);
                            if (sqlite3_step(updateStmt) == SQLITE_DONE) {
                                showInfo("Proteínas atualizadas com sucesso!");
                            } else {
                                showError(QString("Erro ao atualizar: %1").arg(sqlite3_errmsg(db)));
                            }
                            sqlite3_finalize(updateStmt);
                            updateStmt = nullptr;
                        } else {
                            showError(QString("Erro ao preparar atualização: %1").arg(sqlite3_errmsg(db)));
                        }
                    }
                    break;
                }
                case 4: {
                    bool ok;
                    int valor = QInputDialog::getInt(this, "Alterar Carboidrato", "Digite a quantidade de carboidrato:", 0, 0, 10000, 1, &ok);
                    if (ok) {
                        std::string sql = "UPDATE PlanoAlimentar SET Carboidrato = ? WHERE Id = ?";
                        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &updateStmt, nullptr) == SQLITE_OK) {
                            sqlite3_bind_int(updateStmt, 1, valor);
                            sqlite3_bind_int(updateStmt, 2, planoId);
                            if (sqlite3_step(updateStmt) == SQLITE_DONE) {
                                showInfo("Carboidrato atualizado com sucesso!");
                            } else {
                                showError(QString("Erro ao atualizar: %1").arg(sqlite3_errmsg(db)));
                            }
                            sqlite3_finalize(updateStmt);
                            updateStmt = nullptr;
                        } else {
                            showError(QString("Erro ao preparar atualização: %1").arg(sqlite3_errmsg(db)));
                        }
                    }
                    break;
                }
                case 5: {
                    bool ok;
                    int valor = QInputDialog::getInt(this, "Alterar Gordura", "Digite a quantidade de gordura:", 0, 0, 10000, 1, &ok);
                    if (ok) {
                        std::string sql = "UPDATE PlanoAlimentar SET Gordura = ? WHERE Id = ?";
                        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &updateStmt, nullptr) == SQLITE_OK) {
                            sqlite3_bind_int(updateStmt, 1, valor);
                            sqlite3_bind_int(updateStmt, 2, planoId);
                            if (sqlite3_step(updateStmt) == SQLITE_DONE) {
                                showInfo("Gordura atualizada com sucesso!");
                            } else {
                                showError(QString("Erro ao atualizar: %1").arg(sqlite3_errmsg(db)));
                            }
                            sqlite3_finalize(updateStmt);
                            updateStmt = nullptr;
                        } else {
                            showError(QString("Erro ao preparar atualização: %1").arg(sqlite3_errmsg(db)));
                        }
                    }
                    break;
                }
                case 6: {
                    bool ok;
                    QString valor = QInputDialog::getText(this, "Alterar Alimentos Evitados", "Digite os alimentos a evitar:", QLineEdit::Normal, "", &ok);
                    if (ok && !valor.isEmpty()) {
                        std::string sql = "UPDATE PlanoAlimentar SET AlimentosEvitados = ? WHERE Id = ?";
                        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &updateStmt, nullptr) == SQLITE_OK) {
                            sqlite3_bind_text(updateStmt, 1, valor.toStdString().c_str(), -1, SQLITE_TRANSIENT);
                            sqlite3_bind_int(updateStmt, 2, planoId);
                            if (sqlite3_step(updateStmt) == SQLITE_DONE) {
                                showInfo("Alimentos evitados atualizados com sucesso!");
                            } else {
                                showError(QString("Erro ao atualizar: %1").arg(sqlite3_errmsg(db)));
                            }
                            sqlite3_finalize(updateStmt);
                            updateStmt = nullptr;
                        } else {
                            showError(QString("Erro ao preparar atualização: %1").arg(sqlite3_errmsg(db)));
                        }
                    }
                    break;
                }
            }

            if (db) sqlite3_close(db);
        } catch (const std::exception& e) {
            showError(QString("Erro ao alterar plano: %1").arg(e.what()));
            if (db) sqlite3_close(db);
        } catch (...) {
            showError("Erro desconhecido ao alterar plano.");
            if (db) sqlite3_close(db);
        }
    }
}

// Exibir plano alimentar
void MainWindow::onViewMealPlan()
{
    if (!m_currentPatient || m_patientId <= 0) {
        showError("Nenhum paciente logado. Faça login primeiro.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Plano Alimentar");
    dialog.resize(700, 400);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    QTextEdit* textEdit = new QTextEdit(&dialog);
    textEdit->setReadOnly(true);
    textEdit->setFont(QFont("Courier", 10));
    layout->addWidget(textEdit);

    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok, Qt::Horizontal, &dialog);
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);

    sqlite3* database = nullptr;
    sqlite3_stmt* stmt = nullptr;
    
    try {
        QString output;
        
        QString dbPath = getDatabasePath();
        int rc = sqlite3_open(dbPath.toUtf8().constData(), &database);
        if (rc != SQLITE_OK) {
            showError(QString("Erro ao abrir banco de dados: %1").arg(sqlite3_errmsg(database)));
            if (database) sqlite3_close(database);
            return;
        }
        
        const char* query = 
            "SELECT Nutricionista, Vitaminas, Proteinas, Carboidrato, Gordura, AlimentosEvitados "
            "FROM PlanoAlimentar "
            "WHERE Paciente = ?";
        
        if (sqlite3_prepare_v2(database, query, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, m_patientId);
            
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                QString nutricionista = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
                QString vitaminas = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
                int proteinas = sqlite3_column_int(stmt, 2);
                int carboidrato = sqlite3_column_int(stmt, 3);
                int gordura = sqlite3_column_int(stmt, 4);
                QString alimentos = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
                
                output += "=== PLANO ALIMENTAR ===\n\n";
                output += QString("Nutricionista: %1\n").arg(nutricionista);
                output += QString("Vitaminas necessárias: %1\n").arg(vitaminas);
                output += QString("Proteínas: %1 g\n").arg(proteinas);
                output += QString("Carboidratos: %1 g\n").arg(carboidrato);
                output += QString("Gorduras: %1 g\n").arg(gordura);
                output += QString("Alimentos a evitar: %1\n").arg(alimentos);
            } else {
                output = "Nenhum plano alimentar encontrado. Registre um plano primeiro.";
            }
            
            sqlite3_finalize(stmt);
            stmt = nullptr;
        } else {
            showError(QString("Erro ao preparar consulta: %1").arg(sqlite3_errmsg(database)));
            if (database) sqlite3_close(database);
            return;
        }
        
        sqlite3_close(database);
        database = nullptr;
        textEdit->setPlainText(output);
        
    } catch (const std::exception& e) {
        showError(QString("Erro ao buscar plano alimentar: %1").arg(e.what()));
        if (stmt) sqlite3_finalize(stmt);
        if (database) sqlite3_close(database);
        return;
    } catch (...) {
        showError("Erro desconhecido ao buscar plano alimentar.");
        if (stmt) sqlite3_finalize(stmt);
        if (database) sqlite3_close(database);
        return;
    }
    
    dialog.exec();
}

// Inicializa o DB criando todas as tabelas
bool MainWindow::initializeDatabase()
{
    sqlite3* db = nullptr;
    char* errMsg = nullptr;
    
    try {
        // Abre conexao com o DB (cria o arquivo se n existir)
        QString dbPath = getDatabasePath();
        int rc = sqlite3_open(dbPath.toUtf8().constData(), &db);
        if (rc != SQLITE_OK) {
            std::cerr << "Erro ao abrir banco de dados: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return false;
        }
        
        // SQL pra criar todas as tabelas
        const char* sql = 
            "BEGIN TRANSACTION;"
            "CREATE TABLE IF NOT EXISTS \"Medicacao\" ("
            "\"Id\" INTEGER,"
            "\"Paciente\" INTEGER,"
            "\"Nome\" TEXT,"
            "\"Horario\" TEXT,"
            "\"Dosagem\" REAL,"
            "\"Medico\" TEXT,"
            "PRIMARY KEY(\"Id\" AUTOINCREMENT),"
            "FOREIGN KEY(\"Paciente\") REFERENCES \"Paciente\"(\"Id\")"
            ");"
            "CREATE TABLE IF NOT EXISTS \"Paciente\" ("
            "\"Id\" INTEGER,"
            "\"Pessoa\" INTEGER,"
            "\"TipoSanguineo\" TEXT,"
            "\"TipoDiabetes\" TEXT,"
            "\"Peso\" REAL,"
            "\"Altura\" REAL,"
            "PRIMARY KEY(\"Id\" AUTOINCREMENT),"
            "FOREIGN KEY(\"Pessoa\") REFERENCES \"Pessoa\"(\"id\")"
            ");"
            "CREATE TABLE IF NOT EXISTS \"Pessoa\" ("
            "\"id\" INTEGER,"
            "\"Nome\" TEXT,"
            "\"Cpf\" TEXT,"
            "\"Sexo\" TEXT,"
            "\"Senha\" TEXT,"
            "\"Endereco\" TEXT,"
            "\"Idade\" INTEGER,"
            "PRIMARY KEY(\"id\" AUTOINCREMENT)"
            ");"
            "CREATE TABLE IF NOT EXISTS \"PlanoAlimentar\" ("
            "\"Id\" INTEGER,"
            "\"Paciente\" INTEGER,"
            "\"AlimentosEvitados\" TEXT,"
            "\"Proteinas\" INTEGER,"
            "\"Carboidrato\" INTEGER,"
            "\"Gordura\" INTEGER,"
            "\"Vitaminas\" TEXT,"
            "\"Nutricionista\" TEXT,"
            "PRIMARY KEY(\"Id\" AUTOINCREMENT),"
            "FOREIGN KEY(\"Paciente\") REFERENCES \"Paciente\"(\"Id\")"
            ");"
            "CREATE TABLE IF NOT EXISTS \"RegistroConsulta\" ("
            "\"Id\" INTEGER,"
            "\"RegistroSaude\" INTEGER,"
            "\"Medico\" TEXT,"
            "\"Descricao\" TEXT,"
            "\"Local\" TEXT,"
            "\"Especialidade\" TEXT,"
            "PRIMARY KEY(\"Id\" AUTOINCREMENT),"
            "FOREIGN KEY(\"RegistroSaude\") REFERENCES \"RegistroSaude\"(\"Id\")"
            ");"
            "CREATE TABLE IF NOT EXISTS \"RegistroExame\" ("
            "\"Id\" INTEGER,"
            "\"RegistroSaude\" INTEGER,"
            "\"Nome\" TEXT,"
            "\"Medico\" TEXT,"
            "\"Resultado\" TEXT,"
            "\"Laboratorio\" TEXT,"
            "PRIMARY KEY(\"Id\" AUTOINCREMENT),"
            "FOREIGN KEY(\"RegistroSaude\") REFERENCES \"RegistroSaude\"(\"Id\")"
            ");"
            "CREATE TABLE IF NOT EXISTS \"RegistroGlicose\" ("
            "\"Id\" INTEGER,"
            "\"RegistroSaude\" INTEGER,"
            "\"NivelGlicose\" REAL,"
            "\"Jejum\" BLOB,"
            "PRIMARY KEY(\"Id\" AUTOINCREMENT),"
            "FOREIGN KEY(\"RegistroSaude\") REFERENCES \"RegistroSaude\"(\"Id\")"
            ");"
            "CREATE TABLE IF NOT EXISTS \"RegistroMedicacao\" ("
            "\"Id\" INTEGER,"
            "\"RegistroSaude\" INTEGER,"
            "\"Medicacao\" INTEGER,"
            "PRIMARY KEY(\"Id\" AUTOINCREMENT),"
            "FOREIGN KEY(\"Medicacao\") REFERENCES \"Medicacao\"(\"Id\"),"
            "FOREIGN KEY(\"RegistroSaude\") REFERENCES \"RegistroSaude\"(\"Id\")"
            ");"
            "CREATE TABLE IF NOT EXISTS \"RegistroSaude\" ("
            "\"Id\" INTEGER,"
            "\"Paciente\" INTEGER,"
            "\"Hora\" TEXT,"
            "\"Data\" TEXT,"
            "PRIMARY KEY(\"Id\" AUTOINCREMENT),"
            "FOREIGN KEY(\"Paciente\") REFERENCES \"Paciente\"(\"Id\")"
            ");"
            "COMMIT;";
        
        // Executa o SQL
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "Erro ao criar tabelas: " << (errMsg ? errMsg : sqlite3_errmsg(db)) << std::endl;
            if (errMsg) {
                sqlite3_free(errMsg);
            }
            sqlite3_close(db);
            return false;
        }
        
        sqlite3_close(db);
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Exceção ao inicializar banco: " << e.what() << std::endl;
        if (db) {
            sqlite3_close(db);
        }
        return false;
    } catch (...) {
        std::cerr << "Erro desconhecido ao inicializar banco." << std::endl;
        if (db) {
            sqlite3_close(db);
        }
        return false;
    }
}


