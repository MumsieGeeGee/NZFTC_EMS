// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.

#include "Admin_Dash_Home_Page.h"
#include "Header_Bar.h"
#include "Left_Side_Bar_Navigation.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QFont>

void setupAdminDashboardUI(QMainWindow *window)
{
    if (!window) return;

    window->setWindowTitle("NZFTC EMS - Admin Dashboard");
    window->showMaximized();

    QWidget *central = new QWidget(window);
    central->setStyleSheet("background:#F5F7FA;");

    QVBoxLayout *rootLayout = new QVBoxLayout(central);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    auto *header = new HeaderBar(central);
    rootLayout->addWidget(header);
    
    // Content area: left sidebar + main area
    auto *contentLayout = new QHBoxLayout();
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);
    
    auto *sidebar = new Left_Side_Bar_Navigation(central);
    
    auto *mainArea = new QWidget(central);
    mainArea->setStyleSheet("background:#F5F7FA;");
    
    contentLayout->addWidget(sidebar, 0);
    contentLayout->addWidget(mainArea, 1);
    
    rootLayout->addLayout(contentLayout);

    window->setCentralWidget(central);
}
