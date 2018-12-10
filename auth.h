#ifndef AUTH_H
#define AUTH_H

#include "mainwindow.h"
#include "smtp.h"
#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QRegExpValidator>
#include <QRegExp>


namespace Ui {
class Auth;
}

class Auth : public QWidget
{
    Q_OBJECT

public:
    explicit Auth(QWidget *parent = nullptr);
    ~Auth();
    QPoint mpos = QPoint(-1,-1);

private slots:
    void on_sign_in_clicked();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void slotTimer();

    void on_activate_but_clicked();

    void on_cont_demo_clicked();
    void on_register_button_clicked();

private:
    Ui::Auth *ui;
    QSqlDatabase db;
    MainWindow* form;
    QTimer *timer;
    Smtp *smtp;
};

#endif // AUTH_H
