#include <QApplication>
#include <QMainWindow>
#include "../../UI/src/mainUI.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;

    setupLoginUI(&window);

    window.show();
    return app.exec();
}
