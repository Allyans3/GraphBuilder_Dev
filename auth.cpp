#include "auth.h"
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

    QRegExp re("[\\S]{0,}");
    QRegExp email("^[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+@[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?(?:\\.[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)*$");
    QRegExp auth("[0-9]{4}-[0-9]{4}-[0-9]{4}-[0-9]{4}");
    QRegExp auth2("[0-9]{4}");
    ui->login->setValidator(new QRegExpValidator(re, this));
    ui->pass->setValidator(new QRegExpValidator(re, this));
    ui->login_reg->setValidator(new QRegExpValidator(re, this));
    ui->pass_reg->setValidator(new QRegExpValidator(re, this));
    ui->confirm_pass_reg_2->setValidator(new QRegExpValidator(re, this));
    ui->e_mail_reg->setValidator(new QRegExpValidator(email, this));
    ui->auth_line->setValidator(new QRegExpValidator(auth, this));
    ui->e_mail_lost_pass->setValidator(new QRegExpValidator(email, this));
    ui->code_rec->setValidator(new QRegExpValidator(auth2, this));
    ui->pass_rec->setValidator(new QRegExpValidator(re, this));
    ui->pass_conf_rec->setValidator(new QRegExpValidator(re, this));



    connect(ui->sign_up,&QPushButton::clicked,this,[this]{
        ui->stackedWidget->setCurrentIndex(2);
        if(!db.isOpen())
            slotTimer();
    });
    connect(ui->confirm_pass_reg_2,&QLineEdit::textChanged,this,[this]{
        if(ui->confirm_pass_reg_2->text().size() == 0)
            ui->pass_correcting->setText("");
        else if(ui->pass_reg->text() != ui->confirm_pass_reg_2->text())
            ui->pass_correcting->setText("Incorrect");
        else
            ui->pass_correcting->setText("Correct");
    });
    connect(ui->pass_conf_rec,&QLineEdit::textChanged,this,[this]{
        if(ui->pass_conf_rec->text().size() == 0)
            ui->pass_correcting_2->setText("");
        else if(ui->pass_rec->text() != ui->pass_conf_rec->text())
            ui->pass_correcting_2->setText("Incorrect");
        else
            ui->pass_correcting_2->setText("Correct");
    });
    connect(ui->link_pass,&QPushButton::clicked,this,[this]{
        ui->stackedWidget->setCurrentIndex(3);
        if(!db.isOpen())
            slotTimer();
        ui->email_correcting->setText("");
        ui->code_correcting->setText("");
        ui->pass_correcting_2->setText("");
        ui->e_mail_lost_pass->clear();
        ui->code_rec->clear();
        ui->pass_rec->clear();
        ui->pass_conf_rec->clear();
        ui->label_code->setVisible(false);
        ui->code_rec->setVisible(false);
        ui->code_correcting->setVisible(false);
        ui->label_pass_rec->setVisible(false);
        ui->pass_rec->setVisible(false);
        ui->label_min_8->setVisible(false);
        ui->label_pass_conf_rec->setVisible(false);
        ui->pass_conf_rec->setVisible(false);
        ui->pass_correcting_2->setVisible(false);
        ui->button_lost_pass->setText("Send");
    });


    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("db4free.net");
    db.setDatabaseName("graphauth");
    db.setPort(3306);
    db.setUserName("allyans");
    db.setPassword("12345678");
//    db.setHostName("remotemysql.com");
//    db.setDatabaseName("ChL2NiO4hE");
//    db.setPort(3306);
//    db.setUserName("ChL2NiO4hE");
//    db.setPassword("WSooj0h6o1");
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
                    if(qry.exec("SELECT Login, Activation FROM graphauth WHERE Login=\'" + ui->login->text() + "\'"))
                        if(qry.next())
                            ui->left_entries->setText("You have "+ QString::number(qry.value("Activation").toInt()) + " entries left");
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
    QRegExp email("^[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+@[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?(?:\\.[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)*$");
    int p1=0,p2=0,p3=0,p4=0;
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime())+qrand());
    if(qry.exec("SELECT Login FROM graphauth WHERE Login=\'" + ui->login_reg->text() +"\'"))
    {
        if(qry.next())
            ui->status_db->setText("This login is already used!");
        else if(ui->login_reg->text().size() < 6)
            ui->status_db->setText("Login has less than 6 characters.");
        else if(ui->pass_reg->text().size() < 8)
            ui->status_db->setText("Password has less than 8 characters.");
        else if(ui->pass_reg->text() != ui->confirm_pass_reg_2->text())
        {
            if(ui->confirm_pass_reg_2->text().size() == 0)
                ui->status_db->setText("Enter confirmation password.");
            else
                ui->status_db->setText("Incorrect verification password.");
        }
        else
        {
            if(qry.exec("SELECT E_mail FROM graphauth WHERE E_mail=\'" + ui->e_mail_reg->text() +"\'"))
            {
                if(qry.next())
                    ui->status_db->setText("This e-mail is already used!");
                else if(ui->e_mail_reg->text().size() == 0)
                    ui->status_db->setText("Please enter your e-mail address.");
                else if(!email.exactMatch(ui->e_mail_reg->text()))
                    ui->status_db->setText("Incorrect e-mail address format.");
                else if(!ui->reg_chech_human->isChecked())
                    ui->status_db->setText("Confirm that you are human.");
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
                        smtp->sendMail("graphauth@gmail.com", QString(ui->e_mail_reg->text()) , "Successful registration",QString("This is your login information.\r\nLogin: " + ui->login_reg->text()+ "\r\nPassword: " + ui->pass_reg->text() + "\r\nAuthKey: " + QString(QString::number(p1)+"-"+QString::number(p2)+"-"+QString::number(p3)+"-"+QString::number(p4)) + "\r\r"));
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

void Auth::on_button_lost_pass_clicked()
{
    if(!db.isOpen())
        slotTimer();
    QSqlQuery qry;
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime())+qrand());
    if(ui->pass_rec->isVisible())
    {
        if(ui->pass_rec->text().size() < 8)
            ui->status_db->setText("Password has less than 8 characters.");
        else if(qry.exec("UPDATE graphauth SET Password=\'" + ui->pass_rec->text() + "\' WHERE E_mail=\'" + ui->e_mail_lost_pass->text() + "\'"))
        {
            ui->status_db->setText("Password successfully recovered.");
            ui->stackedWidget->setCurrentIndex(0);
        }
    }
    else if(ui->code_rec->isVisible())
    {
        if(ui->code_rec->text().toInt() == code)
        {
            ui->code_correcting->setText("Correct");
            ui->label_pass_rec->setVisible(true);
            ui->pass_rec->setVisible(true);
            ui->label_min_8->setVisible(true);
            ui->label_pass_conf_rec->setVisible(true);
            ui->pass_conf_rec->setVisible(true);
            ui->pass_correcting_2->setVisible(true);
            ui->button_lost_pass->setText("Recover");
        }
        else
            ui->code_correcting->setText("Incorrect");
    }
    else if(qry.exec("SELECT Login FROM graphauth WHERE E_mail=\'" + ui->e_mail_lost_pass->text() + "\'"))
    {
        if(qry.next())
        {
            QString login = QString(qry.value("Login").toString());
            code = qrand()%8999 +1001;
            smtp = new Smtp("graphauth@gmail.com", "ppwnccfozhahrjmj", "smtp.gmail.com");
            smtp->sendMail("graphauth@gmail.com", QString(ui->e_mail_lost_pass->text()) , "Password recovery",QString("Dear " + login + ",\r  You received this email while trying to reset your password.\r  This is your verification code: " + QString::number(code) + "\r\rIf you did not request a password reset for your account no further action is required and you can safely disregard this message."));
            ui->email_correcting->setText("Email Sent");
            ui->label_code->setVisible(true);
            ui->code_rec->setVisible(true);
            ui->code_correcting->setVisible(true);
            ui->button_lost_pass->setText("Continue");
        }
        else
            ui->email_correcting->setText("Please enter your e-mail address");
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

