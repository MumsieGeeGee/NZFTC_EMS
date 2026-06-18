// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.

#include "Header_Bar.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QDateTime>
#include <QFont>

HeaderBar::HeaderBar(QWidget *parent)
    : QWidget(parent)
{
    const QString primary = "#2A5D8F"; // dark blue
    const QString accent = "#5BA6A6";  // teal
    const QString buttonBg = "#1A73E8";
    const QString buttonHover = "#B33A3A";
    const QString textColor = "white";

        setFixedHeight(180);

        auto *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);

        // Top row: left = primary (title), right = accent (session time, label-only)
        auto *topRow = new QWidget(this);
        auto *topLayout = new QHBoxLayout(topRow);
        topLayout->setContentsMargins(0, 0, 0, 0);
        topLayout->setSpacing(0);

        auto *leftTop = new QWidget(topRow);
        leftTop->setStyleSheet(QString("background:%1;").arg(primary));
        auto *leftTopLayout = new QHBoxLayout(leftTop);
        leftTopLayout->setContentsMargins(24, 14, 12, 14);
        leftTopLayout->setSpacing(8);
        auto *title = new QLabel("NZFTC", leftTop);
        QFont titleFont("Lucida Sans", 64, QFont::Bold);
        title->setFont(titleFont);
        title->setStyleSheet("color: #F5F7FA;");
        leftTopLayout->addWidget(title, 0, Qt::AlignLeft | Qt::AlignVCenter);

        auto *rightTop = new QWidget(topRow);
        rightTop->setStyleSheet(QString("background:%1;").arg(accent));
        auto *rightTopLayout = new QHBoxLayout(rightTop);
        rightTopLayout->setContentsMargins(12, 14, 24, 14);
        rightTopLayout->setSpacing(8);
        auto *sessionLabel = new QLabel("Current Session Time:", rightTop);
        QFont sessionFont("Lucida Sans", 18, QFont::Bold);
        sessionLabel->setFont(sessionFont);
        sessionLabel->setStyleSheet("color: #4A4A4A;");
        rightTopLayout->addWidget(sessionLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);

        topLayout->addWidget(leftTop, 1);
        topLayout->addWidget(rightTop, 1);

        mainLayout->addWidget(topRow);

        // Bottom row: left = accent (date/time), right = primary (timeout label + logout)
        auto *bottomRow = new QWidget(this);
        auto *bottomLayout = new QHBoxLayout(bottomRow);
        bottomLayout->setContentsMargins(0, 0, 0, 0);
        bottomLayout->setSpacing(0);

        auto *leftBottom = new QWidget(bottomRow);
        leftBottom->setStyleSheet(QString("background:%1;").arg(accent));
        auto *leftBottomLayout = new QHBoxLayout(leftBottom);
        leftBottomLayout->setContentsMargins(24, 12, 12, 12);
        leftBottomLayout->setSpacing(8);
        auto *dateBlock = new QWidget(leftBottom);
        auto *dateLayout = new QVBoxLayout(dateBlock);
        dateLayout->setContentsMargins(0, 0, 0, 0);
        dateLayout->setSpacing(2);
        // Time/Date Display
        auto *dateValue = new QLabel(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm"), dateBlock);
        dateValue->setStyleSheet("color: #4A4A4A; font-size:18px; font-weight:700;");
        dateLayout->addWidget(dateValue);
        leftBottomLayout->addWidget(dateBlock, 0, Qt::AlignLeft | Qt::AlignVCenter);

        auto *rightBottom = new QWidget(bottomRow);
        rightBottom->setStyleSheet(QString("background:%1;").arg(primary));
        auto *rightBottomLayout = new QHBoxLayout(rightBottom);
        rightBottomLayout->setContentsMargins(12, 12, 24, 12);
        rightBottomLayout->setSpacing(8);

        auto *timeoutBlock = new QWidget(rightBottom);
        auto *timeoutLayout = new QVBoxLayout(timeoutBlock);
        timeoutLayout->setContentsMargins(0, 0, 0, 0);
        timeoutLayout->setSpacing(2);
        auto *timeoutLabel = new QLabel("Session Timeout:", timeoutBlock);
        timeoutLabel->setFont(sessionFont);
        timeoutLabel->setStyleSheet("color: #F5F7FA;");
        timeoutLayout->addWidget(timeoutLabel, 0, Qt::AlignLeft);
        rightBottomLayout->addWidget(timeoutBlock, 0, Qt::AlignLeft | Qt::AlignVCenter);

        rightBottomLayout->addStretch(1);

        auto *logout = new QPushButton("Logout", rightBottom);
        logout->setFont(QFont("Roboto", 16, QFont::Bold));
        logout->setStyleSheet(QString("QPushButton { background:%1; color: white; border: 2px solid #F5F7FA; border-radius:2px; padding:10px 20px; font-size:20px; } QPushButton:hover { background:#B33A3A; }")
                             .arg(buttonBg));
        logout->setFixedSize(220, 44);
        rightBottomLayout->addWidget(logout, 0, Qt::AlignRight | Qt::AlignVCenter);

        bottomLayout->addWidget(leftBottom, 1);
        bottomLayout->addWidget(rightBottom, 1);

        mainLayout->addWidget(bottomRow);

        // live clock update
        auto *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [dateValue]() {
            dateValue->setText(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm"));
        });
        timer->start(1000);
    }
