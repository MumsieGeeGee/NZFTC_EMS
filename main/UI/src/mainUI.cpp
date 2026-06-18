// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
#include "mainUI.h"
#include "../Admin_Dashboard/Home_Page_Admin/Admin_Dash_Home_Page.h"
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
#include <QObject>
#include <QEvent>
#include <QCursor>

void setupLoginUI(QMainWindow *window)
{
    if (!window) return;
    window->setWindowTitle("NZFTC EMS - Login"); // Set window title
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

    // Header 
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

    // Email field
    QFont bodyFont("Roboto", 11);
    QLineEdit *emailEdit = new QLineEdit(body);
    emailEdit->setFixedHeight(36);
    emailEdit->setFont(bodyFont);
    emailEdit->setText("Email:");
    emailEdit->setStyleSheet("QLineEdit { background: white; color: #4A4A4A; border: none; outline: none; padding: 6px; font-size: 14px; } QLineEdit:focus { border: none; outline: none; }");
    emailEdit->setReadOnly(true);

    // Password field
    QLineEdit *passEdit = new QLineEdit(body);
    passEdit->setFixedHeight(36);
    passEdit->setFont(bodyFont);
    passEdit->setText("Password:");
    passEdit->setStyleSheet("QLineEdit { background: white; color: #4A4A4A; border: none; outline: none; padding: 6px; font-size: 14px; } QLineEdit:focus { border: none; outline: none; }");
    passEdit->setReadOnly(true);

    // Forgot Password and Login Buttons
    QSpacerItem *vspacer = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QLabel *forgot = new QLabel("forgot password", body);
    forgot->setStyleSheet("background:#e9e9e9; padding:4px; color:#222; font-size:14px;");
    forgot->setAlignment(Qt::AlignCenter);
    forgot->setFixedWidth(150
);
    forgot->setFont(bodyFont);

    QPushButton *loginBtn = new QPushButton("Login", body);
    loginBtn->setFixedSize(220, 44);
    loginBtn->setStyleSheet(QString("QPushButton { background:%1; color: white; font-size:20px; border: 2px solid #F5F7FA; border-radius:2px; } QPushButton:hover { background:#2E7D6F; }").arg(buttonBg));
    loginBtn->setEnabled(true);
    loginBtn->setFont(QFont("Roboto", 16, QFont::Bold));
    QObject::connect(loginBtn, &QPushButton::clicked, window, [window]() {
        setupAdminDashboardUI(window);
    });

    // Styling and Layout
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

    class HoverFilter : public QObject {
    public:
        HoverFilter(QLineEdit *e, QLineEdit *p, QLabel *f, const QString &eText, const QString &pText)
            : QObject(e), email(e), pass(p), forgot(f), emailText(eText), passText(pText) {}
        bool eventFilter(QObject *obj, QEvent *ev) override {
            if (ev->type() == QEvent::Enter) {
                if (obj == email) {
                    if (email->text() == emailText) email->clear();
                    email->setCursor(Qt::IBeamCursor);
                } else if (obj == pass) {
                    if (pass->text() == passText) pass->clear();
                    pass->setCursor(Qt::IBeamCursor);
                } else if (obj == forgot) {
                    forgot->setStyleSheet("background:#e9e9e9; padding:6px 10px; color:#222; font-size:14px; border:2px inset #4A4A4A; border-radius:2px;");
                }
            } else if (ev->type() == QEvent::Leave) {
                if (obj == email) {
                    if (email->text().isEmpty()) email->setText(emailText);
                    email->unsetCursor();
                } else if (obj == pass) {
                    if (pass->text().isEmpty()) pass->setText(passText);
                    pass->unsetCursor();
                } else if (obj == forgot) {
                    forgot->setStyleSheet("background:#e9e9e9; padding:4px; color:#222; font-size:14px;");
                }
            }
            return QObject::eventFilter(obj, ev);
        }
    private:
        QLineEdit *email;
        QLineEdit *pass;
        QLabel *forgot;
        QString emailText;
        QString passText;
    } *filter = new HoverFilter(emailEdit, passEdit, forgot, QString("Email:"), QString("Password:"));

    emailEdit->installEventFilter(filter);
    passEdit->installEventFilter(filter);
    forgot->installEventFilter(filter);
}
