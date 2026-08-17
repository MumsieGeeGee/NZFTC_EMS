// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.

#ifndef LEFT_SIDE_BAR_NAVIGATION_H
#define LEFT_SIDE_BAR_NAVIGATION_H

#include <QWidget>
#include <QPushButton>
#include <QIcon>
#include <QFont>
#include <QString>

class Left_Side_Bar_Navigation : public QWidget
{
    Q_OBJECT

public:
    explicit Left_Side_Bar_Navigation(QWidget *parent = nullptr);

    // Add a menu button with optional icon. Returns pointer so caller can connect signals.
    QPushButton* addMenuButton(const QString &text, const QIcon &icon = QIcon());
    QPushButton* addMenuButton(const QString &text, const QIcon &icon, const QString &backgroundColor, const QString &textColor);

    // Placeholder to satisfy shared API; sidebar width is fixed independent of header.
    void setWidthFromTitle(const QString &title, const QFont &font, int leftPadding = 24, int rightPadding = 12);
};

#endif // LEFT_SIDE_BAR_NAVIGATION_H
