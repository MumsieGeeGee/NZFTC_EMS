// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.

#include "Employee_Dash_Home_Page.h"
#include "Header_Bar.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QFont>

void setupEmployeeDashboardUI(QMainWindow *window)
{
    if (!window) return;

    window->setWindowTitle("NZFTC EMS - Employee Dashboard");
    window->showMaximized();

    QWidget *central = new QWidget(window);
    central->setStyleSheet("background:#F5F7FA;");

    QVBoxLayout *rootLayout = new QVBoxLayout(central);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    auto *header = new HeaderBar(central);
    rootLayout->addWidget(header);

    rootLayout->addStretch(1);

    window->setCentralWidget(central);
}
