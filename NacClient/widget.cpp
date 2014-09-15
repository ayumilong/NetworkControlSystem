#include <QtGui>
#include <Qt>

#include "widget.h"
#include "common/constants.h"
#include "client/KClient.h"

#include <time.h>
#include <pthread.h>

KClient newClient;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->pt=-1; //Indicate the thread is NULL

    strncpy(this->ccArrLocalAddr, g_ccArrLocalAddr, g_ciAddrStrLen); //Default Server's IP address
    this->ciPort=g_ciPort;  //Default Server's port number

    flag=REFUSED; //The default value indicates one can not login

    this->usualModelButton=new QPushButton(tr("&Usral Model"));
    this->usualModelButton->setFixedSize(150, 50);
    this->fingerprintModelButton=new QPushButton(tr("&Fingerprint Model"));
    this->fingerprintModelButton->setFixedSize(150, 50);
    this->phoneModelButton=new QPushButton(tr("&Phone Model"));
    this->phoneModelButton->setFixedSize(150,50);
    this->modelLabel=new QLabel(tr("<font color='red'>Please selete login model!</font>"));
    this->fingerprintModelLabel=new QLabel("<font color='red'>Fingerprint model is under construction......</font>");
    this->fingerprintModelLabel->setVisible(false);
    this->phoneModelLabel=new QLabel("<font color='red'>Phone model is under construction......</font>");
    this->phoneModelLabel->setVisible(false);

    this->usernameLabel=new QLabel(tr("Username:"));
    this->usernameLabel->setFixedWidth(90);
    this->usernameLabel->setVisible(false);
    this->passwordLabel=new QLabel(tr("Password :"));
    this->passwordLabel->setFixedWidth(90);
    this->passwordLabel->setVisible(false);
    this->usernameLine=new QLineEdit;
    this->usernameLine->setVisible(false);
    this->passwordLine=new QLineEdit;
    this->passwordLine->setVisible(false);
    this->passwordLine->setEchoMode(QLineEdit::Password);

    this->loginButton=new QPushButton(tr("&Login"));
    this->loginButton->setFixedSize(100, 50);
    this->loginButton->setVisible(false);
    this->logoutButton=new QPushButton(tr("&Logout"));
    this->logoutButton->setFixedSize(100, 50);
    this->logoutButton->setVisible(false);
    this->setButton=new QPushButton(tr("&Set"));
    this->setButton->setFixedSize(100,50);
    this->setButton->setVisible(false);
    this->exitButton=new QPushButton(tr("&Exit"));
    this->exitButton->setFixedSize(100, 50);
    this->exitButton->setVisible(false);
    this->aboutButton=new QPushButton(tr("&About"));
    this->aboutButton->setFixedSize(100,50);
    this->aboutButton->setVisible(false);
    this->returnToModelButton=new QPushButton(tr("&Model"));
    this->returnToModelButton->setFixedSize(100,50);
    this->returnToModelButton->setVisible(false);
    this->returnToModelButton2=new QPushButton(tr("&Model"));
    this->returnToModelButton2->setFixedSize(100,50);
    this->returnToModelButton2->setVisible(false);

    this->listeningFile=new QTextEdit;
    //this->listeningFile->setVisible(false);
    this->listeningFile->setReadOnly(true);

    //set interface
    this->ipAddress=new QLabel("IP   :");
    this->ipAddress->setFixedWidth(35);
    this->ipAddress->setVisible(false);
    this->portNumber=new QLabel("Port:");
    this->portNumber->setFixedWidth(35);
    this->portNumber->setVisible(false);
    this->macAddress=new QLabel("Mac :");
    this->macAddress->setFixedWidth(35);
    this->macAddress->setVisible(false);
    this->ipLine=new QLineEdit;
    this->ipLine->setVisible(false);
    this->portLine=new QLineEdit;
    this->portLine->setVisible(false);
    this->macLine=new QLineEdit;
    this->macLine->setVisible(false);
    this->setSubmitButton=new QPushButton(tr("&Submit"));
    this->setSubmitButton->setVisible(false);
    this->setSubmitButton->setFixedSize(100,30);
    this->hideSetButton=new QPushButton(tr("&Hide"));
    this->hideSetButton->setVisible(false);
    this->hideSetButton->setFixedSize(100,30);
    this->setLabel=new QLabel("Set server's ip address and port number.");
    this->setLabel->setVisible(false);


    connect(this->exitButton, SIGNAL(clicked()), this, SLOT(exitClient()));

    connect(this->logoutButton, SIGNAL(clicked()), this, SLOT(logout()));
    connect(this->loginButton,SIGNAL(clicked()), this, SLOT(login()));
    connect(this, SIGNAL(loginSuccess()), this, SLOT(loginOK()));
    connect(this->aboutButton, SIGNAL(clicked()), this, SLOT(showAbout()));

    //connect(this->listeningFile, SIGNAL(textChanged()), this, SLOT(moveEndLisFile()));

    connect(this->setButton, SIGNAL(clicked()), this, SLOT(setServer()));
    connect(this->hideSetButton, SIGNAL(clicked()), this, SLOT(hideSet()));
    connect(this->setSubmitButton, SIGNAL(clicked()), this, SLOT(setServerOk()));

    connect(this->usualModelButton, SIGNAL(clicked()), this, SLOT(showUsualModel()));
    connect(this->returnToModelButton, SIGNAL(clicked()), this, SLOT(returnToSetModel()));

    connect(this->fingerprintModelButton, SIGNAL(clicked()), this, SLOT(showFingerprintModel()));
    connect(this->phoneModelButton, SIGNAL(clicked()), this, SLOT(showPhoneModel()));
    connect(this->returnToModelButton2, SIGNAL(clicked()), this, SLOT(returnToSetModel()));

    QHBoxLayout *modelButtonLayout=new QHBoxLayout;
    modelButtonLayout->addWidget(this->usualModelButton);
    modelButtonLayout->addWidget(this->fingerprintModelButton);
    modelButtonLayout->addWidget(this->phoneModelButton);

    QHBoxLayout *returnLayout=new QHBoxLayout;
    returnLayout->addStretch();
    returnLayout->addWidget(this->returnToModelButton2);
    returnLayout->addStretch();

    QHBoxLayout *modelLabelLayout=new QHBoxLayout;
    modelLabelLayout->addStretch();
    modelLabelLayout->addWidget(this->modelLabel);
    modelLabelLayout->addStretch();

    QHBoxLayout *fModelLabelLayout=new QHBoxLayout;
    fModelLabelLayout->addStretch();
    fModelLabelLayout->addWidget(this->fingerprintModelLabel);
    fModelLabelLayout->addStretch();

    QHBoxLayout *pModelLabelLayout=new QHBoxLayout;
    pModelLabelLayout->addStretch();
    pModelLabelLayout->addWidget(this->phoneModelLabel);
    pModelLabelLayout->addStretch();

    QVBoxLayout *modelLayout=new QVBoxLayout;
    modelLayout->addLayout(modelLabelLayout);
    modelLayout->addStrut(20);
    modelLayout->addLayout(modelButtonLayout);
    modelLayout->addLayout(fModelLabelLayout);
    modelLayout->addLayout(pModelLabelLayout);
    modelLayout->addLayout(returnLayout);


    QHBoxLayout *usrLayout=new QHBoxLayout;
    usrLayout->addWidget(this->usernameLabel);
    usrLayout->addWidget(this->usernameLine);

    QHBoxLayout *passLayout=new QHBoxLayout;
    passLayout->addWidget(this->passwordLabel);
    passLayout->addWidget(this->passwordLine);

    QHBoxLayout *ipLayout=new QHBoxLayout;
    ipLayout->addWidget(this->ipAddress);
    ipLayout->addWidget(this->ipLine);

    QHBoxLayout *portLayout=new QHBoxLayout;
    portLayout->addWidget(this->portNumber);
    portLayout->addWidget(this->portLine);

    QHBoxLayout *macLayout=new QHBoxLayout;
    macLayout->addWidget(this->macAddress);
    macLayout->addWidget(this->macLine);

    QHBoxLayout *setButtonLayout=new QHBoxLayout;
    setButtonLayout->addWidget(this->hideSetButton);
    setButtonLayout->addStretch();
    setButtonLayout->addWidget(this->setLabel);
    setButtonLayout->addStretch();
    setButtonLayout->addWidget(this->setSubmitButton);

    QVBoxLayout *setServerLayout;setServerLayout=new QVBoxLayout;
    setServerLayout->addLayout(ipLayout);
    setServerLayout->addLayout(portLayout);
    setServerLayout->addLayout(macLayout);
    setServerLayout->addLayout(setButtonLayout);

    QHBoxLayout *buttonLayout=new QHBoxLayout;
    buttonLayout->addWidget(this->loginButton);
    buttonLayout->addWidget(this->logoutButton);
    buttonLayout->addWidget(this->aboutButton);
    buttonLayout->addWidget(this->setButton);
    buttonLayout->addWidget(this->returnToModelButton);
    buttonLayout->addWidget(this->exitButton);

    QVBoxLayout *mainLayout=new QVBoxLayout;
    mainLayout->addLayout(modelLayout);
    mainLayout->addWidget(this->listeningFile);
    mainLayout->addLayout(usrLayout);
    mainLayout->addLayout(passLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(setServerLayout);

    this->setLayout(mainLayout);

    newClient.startProtection(this); //Start filter mode to protect your computer
}

Widget::~Widget()
{

}

void *loginNow(void *widget)
{
    newClient.startClient(widget);
    pthread_exit(NULL);
}

void Widget::login()
{
    //this->listeningFile->clear();

    if(this->usernameLine->text().length()<3 || this->passwordLine->text().length()<3)
    {
        QMessageBox::warning(this, "NacClient", "Please input username and password!", QMessageBox::Ok, 0);
        return;
    }

    this->flag=REFUSED; //Default value of flag

    int l1=this->usernameLine->text().length();
    int l2=this->passwordLine->text().length();
    int usrLength=l1<g_ciMaxUserNameLen ? l1 :g_ciMaxUserNameLen;
    int passLength=l2<g_ciMaxPwdLen ? l2 :g_ciMaxPwdLen;
    strncpy(this->username, (this->usernameLine->text()).toLatin1(), usrLength);
    strncpy(this->password, this->passwordLine->text().toLatin1(), passLength);
    this->username[usrLength]='\0';
    this->password[passLength]='\0';


    pthread_attr_t attr;  //The attribute of the thread
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&(this->pt), &attr, &loginNow, (void *)this);
    pthread_attr_destroy(&attr);
    pthread_join(this->pt, NULL);

    if(this->flag==FAIL)  //Fail in login
    {
        //this->listeningFile->setVisible(true);
        QString message;
        time_t t;
        t=time(NULL);
        message.sprintf("<font color='red'>Username/Password pair not exit. Please input again! </font> %s", ctime(&t));
        this->listeningFile->append(message);
        newClient.stopClient(this); //Close the connection
    }else if(this->flag==HASLOGIN)
    {
        //this->listeningFile->setVisible(true);
        QString message;
        time_t t;
        t=time(NULL);
        message.sprintf("<font color='red'>User has login. Repeat login is prohibited! </font> %s", ctime(&t));
        this->listeningFile->append(message);
        newClient.stopClient(this); //Close the connection
    }else if(this->flag==REFUSED) //Connection refused
    {
        //this->listeningFile->setVisible(true);
        //this->listeningFile->clear();
        time_t t;
        t=time(NULL);
        QString s;
        s.sprintf("<font color='red'>Connection is refused </font> in %s.", ctime(&t));
        this->listeningFile->append(s);
    }else if(this->flag==SUCCESS)
    {
        //this->listeningFile->setVisible(true);
        this->usernameLabel->setVisible(false);
        this->usernameLine->setVisible(false);
        this->passwordLabel->setVisible(false);
        this->passwordLine->setVisible(false);
        this->loginButton->setVisible(false);
        this->logoutButton->setVisible(true);
        this->setButton->setVisible(false);
        this->returnToModelButton->setVisible(false);

        QString message;
        time_t t;
        t=time(NULL);
        message.sprintf("<font color='blue'>%s</font>, welcome!----%s", this->username, ctime(&t));
        this->listeningFile->append(message);

        emit this->loginSuccess();
    }else if(this->flag==OTHER)
    {
        //this->listeningFile->setVisible(true);
        this->listeningFile->append("<font color='red'>Error!!</font>");
        newClient.stopClient(this); //Close the connection
    }
}

void *keepLogin(void *widget)
{
    QString message;
    message.sprintf("<font color='red'>Protection has been started.</font>");
    ((Widget *)widget)->getListeningFile()->append(message);
    newClient.keepClient(widget);
    pthread_exit(NULL);
}

void Widget::loginOK()
{
    pthread_attr_t attr;  //The attribute of the thread
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&(this->pt), &attr, &keepLogin, (void *)this);
    pthread_attr_destroy(&attr);
}

void Widget::logout()
{
    int yesOrNo=QMessageBox::question(this, "NacClient", "Are you sure to logout?", QMessageBox::Yes, QMessageBox::No, 0);
    if(yesOrNo==QMessageBox::Yes)
    {
        cout<<"Start stop"<<endl;
        newClient.stopClient(this);
        flag=LOGOUT; //Indicate the user has logout
        pthread_cancel(this->pt);
        cout<<"Logout! Cancel client thread!"<<endl;

	QString message;
        time_t t;
        t=time(NULL);
        message.sprintf("<font color='blue'>%s</font>, logout!----%s", this->username, ctime(&t));
        this->listeningFile->append(message);

        this->usernameLabel->setVisible(true);
        this->usernameLine->setVisible(true);
        this->usernameLine->clear();
        this->passwordLabel->setVisible(true);
        this->passwordLine->setVisible(true);
        this->passwordLine->clear();
        this->loginButton->setVisible(true);
        this->logoutButton->setVisible(false);
        this->setButton->setVisible(true);
        this->returnToModelButton->setVisible(true);
    }
}

void Widget::exitClient()
{
   this->close();
}

void Widget::closeEvent( QCloseEvent* qce )
{
    int yesOrNo=QMessageBox::question(this, "NacClient", "Are you sure to exit?", QMessageBox::Yes, QMessageBox::No);
    if(yesOrNo == QMessageBox::Yes)
    {
        newClient.stopClient(this);
        if(this->pt!=-1)
        {
            pthread_cancel(this->pt);
            cout<<"Exit! Cancle client thread!"<<endl;
        }
        newClient.stopProtection(); //Close filter model
        qce->accept();
    }else
    {
        qce->ignore();
    }
}

void Widget::clearListeningFile()
{
    this->listeningFile->setVisible(false);
    this->listeningFile->clear();
}

void Widget::showAbout()
{
    QMessageBox::about(this, "About NacClient", "NacClient 1.0.0\nBased on Linux & C++ & Qt 4.7.0\nBuilt on May 20 2013\nCopyright 2013-2013 Nac Corporation. All rights reserved.\nAuthor: Qin Peixi, Hao Zhanzhu, Zhang Yaolin");
}

void Widget::setServer()
{
    //this->clearListeningFile();

    this->usernameLabel->setVisible(false);
    this->usernameLine->setVisible(false);
    this->passwordLabel->setVisible(false);
    this->passwordLine->setVisible(false);

    this->loginButton->setVisible(false);
    this->exitButton->setVisible(false);
    this->aboutButton->setVisible(false);
    this->setButton->setVisible(false);
    this->returnToModelButton->setVisible(false);

    this->ipAddress->setVisible(true);
    this->ipLine->setVisible(true);
    this->portNumber->setVisible(true);
    this->portLine->setVisible(true);
    this->macAddress->setVisible(true);
    this->macLine->setVisible(true);
    this->hideSetButton->setVisible(true);
    this->setLabel->setVisible(true);
    this->setSubmitButton->setVisible(true);
}

void Widget::setServerOk()
{
    if(!(this->ipLine->text().isEmpty())) //Set IP address
    {
        int l=this->ipLine->text().length();
        int length=l<g_ciAddrStrLen  ? l :g_ciAddrStrLen ;
        strncpy(this->ccArrLocalAddr, (this->ipLine->text()).toLatin1(), length);
        this->ccArrLocalAddr[length]='\0';
    }
    if(!(this->portLine->text().isEmpty())) //Set port number
    {
        int pNumber=this->portLine->text().toInt();
        if(pNumber<1000 || pNumber>50000) //You input a invalide port number
        {
            QMessageBox::warning(this, "NacClient", "Please input a valid port number!", QMessageBox::Ok, 0);
            return;
        }else
        {
            this->ciPort=pNumber;
        }
    }
    if(!(this->macLine->text().isEmpty()))//Set MAC address
    {
        if(this->macLine->text().length()!=17)
        {
            QMessageBox::warning(this, "Add server's MAC", "Please input a correct server MAC!");
            return;
        }
        QString s=this->macLine->text(); //The MAC that you input
        s=s.toUpper();
        unsigned char newMac[g_ciMaxMacAddrLen]={0x00};
        if(!(this->string_to_unsignedchar(s.toStdString().c_str(), newMac))) //If return false, this indicates the input has error
        {
            QMessageBox::warning(this, "Add server's' MAC", "Please input a correct server MAC!");
            return;
        }
        KMac kmac;
        kmac.SetAddr(newMac);
        newClient.addServerMac(kmac);
    }
    this->usernameLabel->setVisible(true);
    this->usernameLine->setVisible(true);
    this->passwordLabel->setVisible(true);
    this->passwordLine->setVisible(true);

    this->loginButton->setVisible(true);
    this->exitButton->setVisible(true);
    this->aboutButton->setVisible(true);
    this->setButton->setVisible(true);
    this->returnToModelButton->setVisible(true);

    this->ipAddress->setVisible(false);
    this->ipLine->setVisible(false);
    this->portNumber->setVisible(false);
    this->portLine->setVisible(false);
    this->macAddress->setVisible(false);
    this->macLine->setVisible(false);
    this->hideSetButton->setVisible(false);
    this->setLabel->setVisible(false);
    this->setSubmitButton->setVisible(false);
}

void Widget::hideSet()
{
    this->usernameLabel->setVisible(true);
    this->usernameLine->setVisible(true);
    this->passwordLabel->setVisible(true);
    this->passwordLine->setVisible(true);

    this->loginButton->setVisible(true);
    this->exitButton->setVisible(true);
    this->aboutButton->setVisible(true);
    this->setButton->setVisible(true);
    this->returnToModelButton->setVisible(true);

    this->ipAddress->setVisible(false);
    this->ipLine->setVisible(false);
    this->portNumber->setVisible(false);
    this->portLine->setVisible(false);
    this->macAddress->setVisible(false);
    this->macLine->setVisible(false);
    this->hideSetButton->setVisible(false);
    this->setLabel->setVisible(false);
    this->setSubmitButton->setVisible(false);
}

void Widget::showFingerprintModel()
{
    //this->clearListeningFile();

    this->fingerprintModelLabel->setVisible(true);
    this->returnToModelButton2->setVisible(true);

    this->modelLabel->setVisible(false);
    this->usualModelButton->setVisible(false);
    this->fingerprintModelButton->setVisible(false);
    this->phoneModelButton->setVisible(false);
}

void Widget::showPhoneModel()
{
    //this->clearListeningFile();

    this->phoneModelLabel->setVisible(true);
    this->returnToModelButton2->setVisible(true);

    this->modelLabel->setVisible(false);
    this->usualModelButton->setVisible(false);
    this->fingerprintModelButton->setVisible(false);
    this->phoneModelButton->setVisible(false);
}

void Widget::showUsualModel()
{
    //this->clearListeningFile();

    this->usernameLabel->setVisible(true);
    this->usernameLine->setVisible(true);
    this->passwordLabel->setVisible(true);
    this->passwordLine->setVisible(true);

    this->loginButton->setVisible(true);
    this->exitButton->setVisible(true);
    this->aboutButton->setVisible(true);
    this->setButton->setVisible(true);
    this->returnToModelButton->setVisible(true);

    this->modelLabel->setVisible(false);
    this->usualModelButton->setVisible(false);
    this->fingerprintModelButton->setVisible(false);
    this->phoneModelButton->setVisible(false);
}

void Widget::returnToSetModel()
{
    //this->clearListeningFile();

    this->usernameLabel->setVisible(false);
    this->usernameLine->setVisible(false);
    this->passwordLabel->setVisible(false);
    this->passwordLine->setVisible(false);

    this->loginButton->setVisible(false);
    this->exitButton->setVisible(false);
    this->aboutButton->setVisible(false);
    this->setButton->setVisible(false);
    this->returnToModelButton->setVisible(false);

    this->fingerprintModelLabel->setVisible(false);
    this->phoneModelLabel->setVisible(false);
    this->returnToModelButton2->setVisible(false);

    this->modelLabel->setVisible(true);
    this->usualModelButton->setVisible(true);
    this->fingerprintModelButton->setVisible(true);
    this->phoneModelButton->setVisible(true);
}

char *Widget::getUsername()
{
    return this->username;
}

char *Widget::getPassword()
{
    return this->password;
}

enum logFlag Widget::getFlag()
{
    return this->flag;
}

QTextEdit *Widget::getListeningFile()
{
    return this->listeningFile;
}

void Widget::setFlag(enum logFlag flag)
{
    this->flag=flag;
}

char *Widget::getIpAddress()
{
    return this->ccArrLocalAddr;
}

int Widget::getPortNumber()
{
    return this->ciPort;
}

void Widget::appendLisFile(QString s)
{
    this->listeningFile->append(s);
}


void Widget::moveEndLisFile()
{
    QTextCursor cur=this->listeningFile->textCursor();
    cur.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    this->listeningFile->setTextCursor(cur);
}

char digit_convert(char ch)
{
    return (ch-'0');
}

char char_convert(char ch)
{
    return (ch-'A'+10);
}

bool Widget::string_to_unsignedchar(const char *mac,unsigned char newMac[])
{
    int cnt=0;
    for(int i=0;i<17;i=i+3)
    {
            char ch1;
            unsigned char result;
            if(mac[i]>='0' && mac[i]<='9')
            {
                    ch1=(digit_convert(mac[i])<<4);
            }else if(mac[i]>='A' && mac[i]<='F')
            {
                    ch1=(char_convert(mac[i])<<4);
            }else //Input error
            {
                return false;
            }
            if(mac[i+1]>='0' && mac[i+1]<='9')
            {
                    result=(ch1)+(digit_convert(mac[i+1]));
            }
            else if(mac[i+1]>='A' && mac[i+1]<='F')
            {
                    result=(ch1)+(char_convert(mac[i+1]));
            }else //Input error
            {
                return false;
            }
            newMac[cnt++]=result;
    }
    return true;
}
