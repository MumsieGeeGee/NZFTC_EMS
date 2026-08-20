// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
#pragma once
#include <QWidget>
#include <QVector>
#include <QDate>

class QTableWidget;
class QLabel;
class QEvent;

#include "Calendar.h"

class CalendarWidget : public QWidget {
    Q_OBJECT
public:
    explicit CalendarWidget(QWidget *parent = nullptr);

    void setEvents(const QVector<CalendarEvent> &events);

    void setMonth(int year, int month);

signals:
    void dateClicked(const QDate &date);

private:
    void buildUI();
    void populateCalendar();
    void highlightEvents();
    bool eventFilter(QObject *watched, QEvent *event) override;

    QTableWidget *calendarTable;
    QLabel *monthLabel;

    QVector<CalendarEvent> eventList;

    int currentYear;
    int currentMonth;
};
