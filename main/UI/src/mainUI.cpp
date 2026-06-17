#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("NZFTC EMS - Employee Login");
    window.resize(1200, 800);
    window.show();

    return app.exec();
}
