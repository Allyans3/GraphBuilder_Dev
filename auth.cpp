﻿#include "auth.h"
#include "ui_auth.h"

Auth::Auth(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Auth)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    connect(ui->exitbut,&QPushButton::clicked,this,&QWidget::close);
    connect(ui->minimize,&QPushButton::clicked,this,&QWidget::showMinimized);
    connect(ui->back,&QPushButton::clicked,this,[this]{
        ui->stackedWidget->setCurrentIndex(0);
    });
    this->setWindowIcon(QIcon(":/new/prefix1/logo/logo.ico"));

    connect(ui->sign_up,&QPushButton::clicked,this,[this]{
        ui->stackedWidget->setCurrentIndex(2);
        if(!db.isOpen())
            slotTimer();
    });

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("remotemysql.com");
    db.setDatabaseName("ChL2NiO4hE");
    db.setPort(3306);
    db.setUserName("ChL2NiO4hE");
    db.setPassword("WSooj0h6o1");
    db.setConnectOptions("MYSQL_OPT_RECONNECT=TRUE;MYSQL_OPT_CONNECT_TIMEOUT=1;MYSQL_OPT_WRITE_TIMEOUT=1;MYSQL_OPT_READ_TIMEOUT=1");
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &Auth::slotTimer);
    timer->start(100);
}

Auth::~Auth()
{
    delete timer;
    delete ui;
}

void Auth::slotTimer()
{
    timer->stop();
    db.open();
    if(!db.open())
        ui->status_db->setText("Error connecting to database!");
    else
        ui->status_db->setText("Connected to database!");
}

void Auth::on_sign_in_clicked()
{
    if(!db.isOpen())
        slotTimer();
    QSqlQuery qry;
    if(qry.exec("SELECT Login, Password FROM graphauth WHERE Login=\'" + ui->login->text() + "\' AND Password=\'" + ui->pass->text() + "\'"))
    {
        if(qry.next())
        {
            if(qry.exec("SELECT Login, Activation FROM graphauth WHERE Login=\'" + ui->login->text() + "\' AND Activation=\'activated\'"))
            {
                if(qry.next())
                {
                    ui->status_db->setText("All right!");
                    form = new MainWindow();
                    form->show();
                    close();
                }
                else
                {
                    ui->status_db->setText("Need activation!");
                    ui->stackedWidget->setCurrentIndex(1);
                }
            }
        }
        else
            ui->status_db->setText("Wrong login or password!");
    }
}

void Auth::on_register_button_clicked()
{
    if(!db.isOpen())
        slotTimer();
    QSqlQuery qry;
    int p1=0,p2=0,p3=0,p4=0;
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime())+qrand());
    if(qry.exec("SELECT Login FROM graphauth WHERE Login=\'" + ui->login_reg->text() +"\'"))
    {
        if(qry.next())
            ui->status_db->setText("This login is already used!");
        else
        {
            if(qry.exec("SELECT E_mail FROM graphauth WHERE E_mail=\'" + ui->e_mail_reg->text() +"\'"))
            {
                if(qry.next())
                    ui->status_db->setText("This e-mail is already used!");
                else
                {
                    p1 = qrand()%8999 +1001;
                    p2 = qrand()%8999 +1001;
                    p3 = qrand()%8999 +1001;
                    p4 = qrand()%8999 +1001;
                    if(qry.exec("INSERT INTO graphauth (E_mail,Login,Password,KeyAuth,Activation) VALUES ('" + ui->e_mail_reg->text() + "','"+ ui->login_reg->text() + "','" + ui->pass_reg->text()+
                                "','" +QString(QString::number(p1)+"-"+QString::number(p2)+"-"+QString::number(p3)+"-"+QString::number(p4))+"','30')"))
                    {
                        smtp = new Smtp("graphauth@gmail.com", "ppwnccfozhahrjmj", "smtp.gmail.com");
                        smtp->sendMail("graphauth@gmail.com", QString(ui->e_mail_reg->text()) , "Successful registration",QString("This is your login information.\r\nLogin: " + ui->login_reg->text()+ "\r\nPassword: " + ui->pass_reg->text() + "\r\n"));
                        ui->status_db->setText("You have successfully signed up!");
                        ui->login->setText(QString(ui->login_reg->text()));
                        ui->stackedWidget->setCurrentIndex(0);
                    }
                }
            }
        }
    }
}

void Auth::on_activate_but_clicked()
{
    if(!db.isOpen())
        slotTimer();
    QSqlQuery qry;
    if(qry.exec("SELECT Login, KeyAuth FROM graphauth WHERE Login=\'" + ui->login->text() + "\' AND KeyAuth=\'"+ui->auth_line->text()+"\'"))
    {
        if(qry.next())
        {
            qry.exec("UPDATE graphauth SET Activation=\'activated\' WHERE Login=\'" + ui->login->text() + "\'");
            form = new MainWindow();
            form->show();
            close();
        }
        else
            ui->status_db->setText("Incorrect AuthKey! Try again!");
    }
}

void Auth::on_cont_demo_clicked()
{
    if(!db.isOpen())
        slotTimer();
    QSqlQuery qry;
    if(qry.exec("SELECT Login, Activation FROM graphauth WHERE Login=\'" + ui->login->text() + "\' AND Activation=0"))
    {
        if(qry.next())
            ui->status_db->setText("Your trial period has ended! Activate the product!");
        else
        {
            if(qry.exec("UPDATE graphauth SET Activation= Activation-1 WHERE Login=\'" + ui->login->text() + "\'"))
            {
                form = new MainWindow();
                form->show();
                close();
            }
        }
    }
}

void Auth::mousePressEvent(QMouseEvent *event)
{
    if(event->pos().y() <= 35)
        mpos = event->pos();
}

void Auth::mouseMoveEvent(QMouseEvent *event)
{
    if (mpos.x() >= 0 && event->buttons() && Qt::LeftButton)
    {
        QPoint diff = event->pos() - mpos;
        QPoint newpos = this->pos() + diff;

        this->move(newpos);
    }
}

void Auth::mouseReleaseEvent(QMouseEvent *)
{
    mpos = QPoint(-1, -1);
}
