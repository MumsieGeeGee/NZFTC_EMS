#include "mainUI.h"
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QFrame>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QFont>
#include <QString>
void setupLoginUI(QMainWindow *window)
{
    if (!window) return;
    window->setWindowTitle("NZFTC EMS - Login");
    window->resize(1200, 800);

    // Color scheme
    const QString bg = "#C7CED6";       // page background
    const QString cardBorder = "#F5F7FA"; // accent border for the card
    const QString headerBg = "#2A5D8F"; // top band
    const QString bodyBg = "#5BA6A6";   // card body
    const QString textColor = "#4A4A4A"; // text
    const QString buttonBg = "#1A73E8"; // login button

    QWidget *central = new QWidget(window);
    central->setStyleSheet(QString("background:%1;").arg(bg));

    QVBoxLayout *root = new QVBoxLayout(central);
    root->setContentsMargins(40, 40, 40, 40);
    root->setAlignment(Qt::AlignCenter);

    // Card
    QFrame *card = new QFrame(central);
    card->setFixedSize(820, 480);
    card->setFrameShape(QFrame::NoFrame);
    card->setStyleSheet(QString("background:transparent; border: 1px solid %1;")
                        .arg(cardBorder));

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setSpacing(0);
    cardLayout->setContentsMargins(0,0,0,0);

    // Header band
    QLabel *header = new QLabel("NZFTC", card);
    header->setFixedHeight(70);
    QFont hfont("Lucida Sans", 28, QFont::Bold);
    header->setFont(hfont);
    header->setAlignment(Qt::AlignCenter);
    header->setStyleSheet(QString("background:%1; color: white; font-weight: 700;").arg(headerBg));
    cardLayout->addWidget(header);

    // Body
    QFrame *body = new QFrame(card);
    body->setStyleSheet(QString("background:%1;").arg(bodyBg));
    QVBoxLayout *bodyLayout = new QVBoxLayout(body);
    bodyLayout->setContentsMargins(120, 40, 120, 40);
    bodyLayout->setSpacing(18);
    bodyLayout->addSpacing(12);

    // Email field with placeholder text inside
    QFont bodyFont("Roboto", 11);
    QLineEdit *emailEdit = new QLineEdit(body);
    emailEdit->setFixedHeight(36);
    emailEdit->setFont(bodyFont);
    emailEdit->setText("Email:");
    emailEdit->setStyleSheet("QLineEdit { background: white; border: none; outline: none; padding: 6px; font-size: 14px; } QLineEdit:focus { border: none; outline: none; }");
    emailEdit->setEnabled(false);


    // Password field with placeholder text inside
    QLineEdit *passEdit = new QLineEdit(body);
    passEdit->setFixedHeight(36);
    passEdit->setFont(bodyFont);
    passEdit->setText("Password:");
    passEdit->setStyleSheet("QLineEdit { background: white; border: none; outline: none; padding: 6px; font-size: 14px; } QLineEdit:focus { border: none; outline: none; }");
    passEdit->setEnabled(false);

    // Spacer
    QSpacerItem *vspacer = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Expanding);

    QLabel *forgot = new QLabel("forgot password", body);
    forgot->setStyleSheet("background:#e9e9e9; padding:4px; color:#222; font-size:14px;");
    forgot->setAlignment(Qt::AlignCenter);
    forgot->setFixedWidth(150
);
    forgot->setFont(bodyFont);

    QPushButton *loginBtn = new QPushButton("Login", body);
    loginBtn->setFixedSize(220, 44);
    loginBtn->setStyleSheet(QString("background:%1; color: white; font-size:20px; border: 2px solid #F5F7FA; border-radius:2px;").arg(buttonBg));
    loginBtn->setEnabled(false);
    loginBtn->setFont(QFont("Roboto", 16, QFont::Bold));

    // Assemble body
    bodyLayout ->addSpacing(35);
    bodyLayout->addWidget(emailEdit);
    bodyLayout->addWidget(passEdit);
    bodyLayout->addItem(vspacer);
    bodyLayout->addWidget(forgot, 0, Qt::AlignHCenter);
    bodyLayout->addSpacing(50);
    bodyLayout->addWidget(loginBtn, 0, Qt::AlignHCenter);

    cardLayout->addWidget(body);

    root->addWidget(card, 0, Qt::AlignCenter);

    window->setCentralWidget(central);
}


