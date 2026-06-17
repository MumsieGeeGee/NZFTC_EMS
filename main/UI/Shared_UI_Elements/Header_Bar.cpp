// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.

#include "Header_Bar.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFont>

HeaderBar::HeaderBar(QWidget *parent)
    : QWidget(parent)
{
    const QString primary = "#2A5D8F";
    const QString accent = "#5BA6A6";
    const QString panel = "#4A4A4A";
    const QString text = "white";
    const QString logoutHover = "#B33A3A";

    setStyleSheet(QString("background:%1;").arg(primary));
    setFixedHeight(120);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 0, 16, 0);
    mainLayout->setSpacing(8);

    auto *topRow = new QHBoxLayout();
    topRow->setSpacing(12);

    auto *title = new QLabel("NZFTC", this);
    QFont titleFont("Lucida Sans", 28, QFont::Bold);
    title->setFont(titleFont);
    title->setStyleSheet("color:white;");
    title->setFixedWidth(240);
    topRow->addWidget(title, 0, Qt::AlignVCenter);
    topRow->addStretch(1);

    auto *logout = new QPushButton("Logout", this);
    logout->setStyleSheet(QString("QPushButton { background:#1A73E8; color:%1; border:none; padding:8px 18px; font-size:14px; border-radius:2px; } QPushButton:hover { background:%2; }")
                         .arg(text).arg(logoutHover));
    logout->setFixedSize(110, 36);
    topRow->addWidget(logout, 0, Qt::AlignVCenter);

    mainLayout->addLayout(topRow);

    auto *infoBar = new QWidget(this);
    infoBar->setStyleSheet(QString("background:%1; border-radius: 4px;").arg(accent));
    infoBar->setFixedHeight(56);

    auto *infoLayout = new QHBoxLayout(infoBar);
    infoLayout->setContentsMargins(24, 8, 24, 8);
    infoLayout->setSpacing(24);

    auto *dateBlock = new QWidget(infoBar);
    auto *dateLayout = new QVBoxLayout(dateBlock);
    dateLayout->setContentsMargins(0,0,0,0);
    dateLayout->setSpacing(2);
    auto *dateLabel = new QLabel("Time/Date", dateBlock);
    dateLabel->setStyleSheet("color:white; font-size:11px;");
    auto *dateValue = new QLabel("14 Jun 2026 10:16", dateBlock);
    dateValue->setStyleSheet("color:white; font-size:16px; font-weight:600;");
    dateLayout->addWidget(dateLabel);
    dateLayout->addWidget(dateValue);
    infoLayout->addWidget(dateBlock, 1);

    auto *sessionTimeBlock = new QWidget(infoBar);
    auto *sessionTimeLayout = new QVBoxLayout(sessionTimeBlock);
    sessionTimeLayout->setContentsMargins(0,0,0,0);
    sessionTimeLayout->setSpacing(2);
    auto *sessionTimeLabel = new QLabel("Current Session Time", sessionTimeBlock);
    sessionTimeLabel->setStyleSheet("color:white; font-size:11px;");
    auto *sessionTimeValue = new QLabel("1hr 45 min", sessionTimeBlock);
    sessionTimeValue->setStyleSheet("color:white; font-size:16px; font-weight:600;");
    sessionTimeLayout->addWidget(sessionTimeLabel);
    sessionTimeLayout->addWidget(sessionTimeValue);
    infoLayout->addWidget(sessionTimeBlock, 1);

    auto *expiryBlock = new QWidget(infoBar);
    auto *expiryLayout = new QVBoxLayout(expiryBlock);
    expiryLayout->setContentsMargins(0,0,0,0);
    expiryLayout->setSpacing(2);
    auto *expiryLabel = new QLabel("Session Expiry", expiryBlock);
    expiryLabel->setStyleSheet("color:white; font-size:11px;");
    auto *expiryValue = new QLabel("15 min", expiryBlock);
    expiryValue->setStyleSheet("color:white; font-size:16px; font-weight:600;");
    expiryLayout->addWidget(expiryLabel);
    expiryLayout->addWidget(expiryValue);
    infoLayout->addWidget(expiryBlock, 1);

    mainLayout->addWidget(infoBar);
}
