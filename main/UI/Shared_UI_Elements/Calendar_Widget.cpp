#include "Calendar_Widget.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QEvent>
#include <QVariant>
#include <QLocale>

CalendarWidget::CalendarWidget(QWidget *parent)
    : QWidget(parent),
      currentYear(QDate::currentDate().year()),
      currentMonth(QDate::currentDate().month())
{
    buildUI();
    populateCalendar();
}

void CalendarWidget::buildUI()
{
    auto *layout = new QVBoxLayout(this);

    // Header (no navigation buttons)
    monthLabel = new QLabel();
    monthLabel->setAlignment(Qt::AlignCenter);
    QFont headerFont = monthLabel->font();
    headerFont.setBold(true);
    headerFont.setPointSize(headerFont.pointSize() + 2);
    monthLabel->setFont(headerFont);

    // Calendar grid (compact)
    calendarTable = new QTableWidget(6, 7, this);
    calendarTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    calendarTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    calendarTable->verticalHeader()->setVisible(false);
    calendarTable->setSelectionMode(QAbstractItemView::NoSelection);
    calendarTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    calendarTable->setFixedSize(340, 280);

    QStringList days = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
    calendarTable->setHorizontalHeaderLabels(days);

    QWidget *container = new QWidget(this);
    container->setFixedSize(360, 330);
    auto *containerLayout = new QVBoxLayout(container);
    containerLayout->setContentsMargins(6,6,6,6);
    containerLayout->setSpacing(6);
    containerLayout->addWidget(monthLabel, 0, Qt::AlignHCenter);
    containerLayout->addWidget(calendarTable, 0, Qt::AlignHCenter);

    layout->addWidget(container, 0, Qt::AlignHCenter);

    // Day click signal (we use cell widgets and store day as property)
    connect(calendarTable, &QTableWidget::cellClicked, this, [this](int row, int col) {
        QWidget *w = calendarTable->cellWidget(row, col);
        if (!w) return;
        QVariant v = w->property("day");
        if (!v.isValid()) return;
        int day = v.toInt();
        if (day <= 0) return;
        int displayMonth = w->property("month").toInt();
        int displayYear = w->property("year").toInt();
        emit dateClicked(QDate(displayYear, displayMonth, day));
    });
}

void CalendarWidget::populateCalendar()
{
    // Determine if the month spills into the next month (to show "June/July")
    QDate firstDay(currentYear, currentMonth, 1);
    int startColumn = firstDay.dayOfWeek() - 1; // Monday=0
    int daysInMonth = firstDay.daysInMonth();

    // Compute last cell's month
    int totalCells = 6 * 7;
    int occupied = startColumn + daysInMonth;
    bool spills = occupied > (5 * 7); // if fills into the 6th row

    QString header;
    if (spills) {
        int nextMonth = (currentMonth % 12) + 1;
        header = QLocale::system().standaloneMonthName(currentMonth) + "/" + QLocale::system().standaloneMonthName(nextMonth) + " " + QString::number(currentYear);
    } else {
        header = QLocale::system().standaloneMonthName(currentMonth) + " " + QString::number(currentYear);
    }
    monthLabel->setText(header);

    // Clear previous widgets
    calendarTable->clearContents();
    for (int r = 0; r < 6; ++r) {
        for (int c = 0; c < 7; ++c) {
            QWidget *old = calendarTable->cellWidget(r, c);
            if (old) {
                calendarTable->removeCellWidget(r, c);
                delete old;
            }
        }
    }

    int row = 0;
    int col = startColumn;

    for (int day = 1; day <= daysInMonth; ++day) {
        // create a compact cell widget with a small black date box
        QWidget *cell = new QWidget();
        cell->setProperty("day", day);
        cell->setProperty("month", currentMonth);
        cell->setProperty("year", currentYear);
        cell->setStyleSheet("background: white; border: 1px solid #cfcfcf;");

        auto *v = new QVBoxLayout(cell);
        v->setContentsMargins(4,4,4,4);
        v->setSpacing(0);
        QLabel *dot = new QLabel(QString::number(day), cell);
        dot->setFixedSize(18,18);
        dot->setAlignment(Qt::AlignCenter);
        dot->setStyleSheet("background: rgb(0,0,0); color: white; border-radius:3px; font-weight:600;");
        QFont dotFont = dot->font();
        dotFont.setPointSize(dotFont.pointSize() - 2);
        dot->setFont(dotFont);
        v->addWidget(dot, 0, Qt::AlignRight | Qt::AlignTop);
        v->addStretch();

        cell->installEventFilter(this);
        calendarTable->setCellWidget(row, col, cell);

        if (++col > 6) { col = 0; row++; }
    }

    // For remaining cells (next month days), show continuing days without numbers but store properties
    int nextDay = 1;
    int nextMonth = (currentMonth % 12) + 1;
    int nextYear = currentYear + (nextMonth==1 ? 1 : 0);
    for (int i = row; i < 6; ++i) {
        for (int j = col; j < 7; ++j) {
            if (calendarTable->cellWidget(i,j)) continue;
            QWidget *cell = new QWidget();
            cell->setProperty("day", nextDay);
            cell->setProperty("month", nextMonth);
            cell->setProperty("year", nextYear);
            cell->setStyleSheet("background: white; border: 1px solid #cfcfcf;");
            auto *v = new QVBoxLayout(cell);
            v->setContentsMargins(4,4,4,4);
            QLabel *dot = new QLabel(QString::number(nextDay), cell);
            dot->setFixedSize(18,18);
            dot->setAlignment(Qt::AlignCenter);
            dot->setStyleSheet("background: rgba(0,0,0,153); color: white; border-radius:3px; font-weight:600;");
            QFont ndFont = dot->font();
            ndFont.setPointSize(ndFont.pointSize() - 2);
            dot->setFont(ndFont);
            v->addWidget(dot, 0, Qt::AlignRight | Qt::AlignTop);
            v->addStretch();
            cell->installEventFilter(this);
            calendarTable->setCellWidget(i,j, cell);
            nextDay++;
        }
        col = 0;
    }

    highlightEvents();
}

void CalendarWidget::highlightEvents()
{
    for (const auto &event : eventList) {
        // find matching cell widget by comparing properties
        for (int r = 0; r < 6; ++r) {
            for (int c = 0; c < 7; ++c) {
                QWidget *w = calendarTable->cellWidget(r, c);
                if (!w) continue;
                int dy = w->property("day").toInt();
                int my = w->property("month").toInt();
                int yy = w->property("year").toInt();
                if (dy == event.date.day() && my == event.date.month() && yy == event.date.year()) {
                    // add a bottom color bar to indicate event
                    QLabel *bar = new QLabel(w);
                    bar->setFixedHeight(6);
                    bar->setStyleSheet(QString("background:%1;").arg(event.color.name()));
                    auto *lay = qobject_cast<QVBoxLayout*>(w->layout());
                    if (lay) lay->addWidget(bar, 0, Qt::AlignBottom);
                }
            }
        }
    }
}

void CalendarWidget::setEvents(const QVector<CalendarEvent> &events)
{
    eventList = events;
    highlightEvents();
}

void CalendarWidget::setMonth(int year, int month)
{
    currentYear = year;
    currentMonth = month;
    populateCalendar();
}

bool CalendarWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (!watched) return false;
    QWidget *w = qobject_cast<QWidget*>(watched);
    if (!w) return false;

    if (event->type() == QEvent::Enter) {
        w->setStyleSheet("background: #eef6ff; border: 1px solid #bfcfe0;");
        return true;
    } else if (event->type() == QEvent::Leave) {
        w->setStyleSheet("background: white; border: 1px solid #cfcfcf;");
        return true;
    }

    return QWidget::eventFilter(watched, event);
}
