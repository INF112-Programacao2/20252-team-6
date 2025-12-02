#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include <memory>
#include "../include/Patient.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Slot do botao de login
    void onLoginClicked();

    // Acoes pos-login
    void onMarkConsultation();
    void onCreateAccountClicked();
    
    // Consultas
    void onViewConsultations();
    
    // Exames
    void onMarkExam();
    void onViewExams();
    
    // Glicose
    void onRegisterGlucose();
    void onViewGlucose();
    
    // Plano Alimentar
    void onRegisterMealPlan();
    void onChangeMealPlan();
    void onViewMealPlan();

private:
    Ui::MainWindow *ui;

    // Paciente logado (unique_ptr gerencia memoria sozinho)
    std::unique_ptr<Patient> m_currentPatient = nullptr;
    int m_patientId = -1;

    // Helpers
    void showError(const QString& msg);
    void showInfo(const QString& msg);
    
    // Retorna o caminho do DB
    static QString getDatabasePath();
    
    // Inicializa o DB criando todas as tabelas
    static bool initializeDatabase();
};

#endif // MAINWINDOW_HPP

