// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.

#include "Admin_Dash_Home_Page.h"
#include "../../Shared_UI_Elements/Header_Bar.h"
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

    // Leave the shared header as the only content for this shared UI view.
    rootLayout->addStretch(1);

    window->setCentralWidget(central);
}
