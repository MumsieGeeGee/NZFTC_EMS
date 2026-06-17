// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
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
