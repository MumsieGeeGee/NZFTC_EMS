#include <QApplication>
#include <QMainWindow>
#include "../../UI/Shared_UI_Elements/Calendar_Widget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("Calendar Demo");
    window.resize(900, 700);

    CalendarWidget *calendar = new CalendarWidget();

    window.setCentralWidget(calendar);

    window.show();
    return app.exec();
}
