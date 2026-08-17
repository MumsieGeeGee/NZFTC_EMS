// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.

#include "Left_Side_Bar_Navigation.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QSize>

Left_Side_Bar_Navigation::Left_Side_Bar_Navigation(QWidget *parent) : QWidget(parent)
{
    const QString primary = "#2A5D8F"; // dark blue - background of sidebar
    const QString accent = "#F5F7FA"; // Ice Blue - Highlight/Active
    const QString secondary = "#5BA6A6";  // teal - Inactive
    const QString textColor = "#4A4A4A"; // dark gray - text color

    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet(QString("background-color:%1; color:%2;").arg(primary).arg(textColor));

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 12);
    layout->setSpacing(20);
    layout->setAlignment(Qt::AlignTop);

    addMenuButton("menu1", QIcon(), secondary, textColor);
    addMenuButton("menu2", QIcon(), secondary, textColor);
    addMenuButton("menu3", QIcon(), secondary, textColor);

    layout->addStretch(1);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    setFixedWidth(118);
}

QPushButton* Left_Side_Bar_Navigation::addMenuButton(const QString &name, const QIcon &icon)
{
    return addMenuButton(name, icon, "#5BA6A6", "#4A4A4A");
}

QPushButton* Left_Side_Bar_Navigation::addMenuButton(const QString &name, const QIcon &icon, const QString &backgroundColor, const QString &textColor)
{
    const QString buttonHover = "#C7CED6"; // Light Gray - Button Hover

    auto *btn = new QPushButton(this);
    btn->setObjectName(name);
    btn->setText(name);
    btn->setToolTip(name);
    btn->setIcon(icon);
    btn->setIconSize(QSize(32, 32));
    btn->setCursor(Qt::PointingHandCursor);
    btn->setFlat(true);
    btn->setStyleSheet(QString(
        "QPushButton { background-color: %1; color: %2; border: 1px solid rgba(245,247,250,0.55); border-radius: 8px; min-width: 80px; min-height: 80px; max-width: 80px; max-height: 80px; padding: 10px 8px; text-align: center; font-size: 13px; font-weight: 700; }"
        "QPushButton:hover { background-color: %3; }"
        "QPushButton:pressed { background-color: rgba(255,255,255,0.26); }"
    ).arg(backgroundColor, textColor, buttonHover));
    btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    auto *lay = qobject_cast<QVBoxLayout*>(layout());
    if (lay)
        lay->insertWidget(lay->count() - 1, btn, 0, Qt::AlignHCenter | Qt::AlignTop);

    return btn;
}

void Left_Side_Bar_Navigation::setWidthFromTitle(const QString &, const QFont &, int, int)
{
    // This shared sidebar has a fixed narrow width and does not depend on header text.
}
