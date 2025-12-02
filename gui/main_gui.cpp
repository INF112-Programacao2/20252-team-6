#include <QApplication>
#include "MainWindow.hpp"
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Aplica estilo Fusion (mais moderno)
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // Cria e mostra a janela principal
    MainWindow window;
    window.show();
    
    return app.exec();
}

