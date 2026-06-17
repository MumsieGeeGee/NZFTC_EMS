#pragma once
#include <QString>
#include <QDate>
#include <QColor>

struct Calendar {
    QDate date;
    QString title;
    QString type;
    QString details;
    QColor color;
};
