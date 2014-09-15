#include "dialog.h"
#include <QtGui>
#include <Qt>

#include "server/KServer.h"
#include "common/KWhiteList.h"

#include <tchdb.h>
#include <pthread.h>
#include <time.h>
#include <string>

KServer newServer;
pthread_t pt;

Dialog::Dialog(QWidget *parent)
    : QWidget(parent)
{
    this->userLogFile.InitLogCfg(LEVEL_DEBUG, "log/userLogFile", 5*1024*1024);
    this->serverLogFile.InitLogCfg(LEVEL_DEBUG, "log/serverLogFile", 5*1024*1024);
    this->whitelistLogFile.InitLogCfg(LEVEL_DEBUG, "log/whitelistLogFile", 5*1024*1024);

    this->serverEncryptMode=CBC; //Default encrypt mode
    this->whitelisIssuePolicy=BROADCAST; //Indicate the server will broadcast the new MAC to all the clients
    this->startOrNot=STOP; //Indicate the server has not been started
    //The default value of IP and port
    memcpy(this->ccArrLocalAddr, g_ccArrLocalAddr, g_ciAddrStrLen);
    this->ciPort=5000;
    //The four button
    this->startButton=new QPushButton(tr("&Start"));
    this->startButton->setFixedSize(100, 50);
    this->stopButton=new QPushButton(tr("&Stop"));
    this->stopButton->setEnabled(false);
    this->stopButton->setFixedSize(100, 50);
    this->setButton=new QPushButton(tr("&Set"));
    this->setButton->setFixedSize(100, 50);
    this->logButton=new QPushButton(tr("&Log"));
    this->logButton->setFixedSize(100, 50);
    this->whitelistButton=new QPushButton(tr("&Whitelist"));
    this->whitelistButton->setFixedSize(100, 50);
    this->exitButton=new QPushButton(tr("&Exit"));
    this->exitButton->setFixedSize(100, 50);
    this->userInfoButton=new QPushButton(tr("&Users"));
    this->userInfoButton->setFixedSize(100,50);
    this->aboutButton=new QPushButton(tr("&About"));
    this->aboutButton->setFixedSize(100,50);

    this->hideWhitelistButton=new QPushButton(tr("&Hide"));
    this->hideWhitelistButton->setFixedSize(100,30);
    this->hideWhitelistButton->setVisible(false);
    this->addMacButton=new QPushButton(tr("&Add"));
    this->addMacButton->setFixedSize(100, 30);
    this->addMacButton->setVisible(false);
    this->deleteMacButton=new QPushButton(tr("&Delete"));
    this->deleteMacButton->setFixedSize(100,30);
    this->deleteMacButton->setVisible(false);
    this->submitButton=new QPushButton(tr("&Submit"));
    this->submitButton->setFixedSize(100, 30);
    this->submitButton->setVisible(false);
    this->hideLogButton=new QPushButton(tr("&Hide"));
    this->hideLogButton->setFixedSize(100,30);
    this->hideLogButton->setVisible(false);
    this->hideSetButton=new QPushButton(tr("&Hide"));
    this->hideSetButton->setFixedSize(100,30);
    this->hideSetButton->setVisible(false);
    this->hideUserInfoButton=new QPushButton(tr("&Hide"));
    this->hideUserInfoButton->setFixedSize(100,30);
    this->hideUserInfoButton->setVisible(false);
    this->addUserButton=new QPushButton(tr("&Add"));
    this->addUserButton->setFixedSize(100,30);
    this->addUserButton->setVisible(false);
    this->deleteUserButton=new QPushButton(tr("&Delete"));
    this->deleteUserButton->setFixedSize(100,30);
    this->deleteUserButton->setVisible(false);
    this->userLogButton=new QPushButton(tr("&User"));
    this->userLogButton->setFixedSize(100,30);
    this->userLogButton->setVisible(false);
    this->serverLogButton=new QPushButton(tr("&Server"));
    this->serverLogButton->setFixedSize(100,30);
    this->serverLogButton->setVisible(false);
    this->whitelistLogButton=new QPushButton(tr("Whitelist"));
    this->whitelistLogButton->setFixedSize(100,30);
    this->whitelistLogButton->setVisible(false);

    this->ipAddress=new QLabel("IP   :");
    this->ipAddress->setFixedWidth(35);
    this->ipAddress->setVisible(false);
    this->portNumber=new QLabel("Port:");
    this->portNumber->setFixedWidth(35);
    this->portNumber->setVisible(false);
    this->ipLine=new QLineEdit;
    this->ipLine->setVisible(false);
    this->portLine=new QLineEdit;
    this->portLine->setVisible(false);

    this->macAddress=new QLabel("MAC:");
    this->macAddress->setFixedWidth(40);
    this->macAddress->setVisible(false);
    this->macLine=new QLineEdit;
    this->macLine->setVisible(false);
    this->sampleMacLabel=new QLabel("<font color='green'>Sample input: 00:01:0B:1C:04:05</font>");
    this->sampleMacLabel->setVisible(false);

    this->logLabel=new QLabel("<font color='blue'>Display log file, including server, user and whitelist.</font>");
    this->logLabel->setVisible(false);
    this->whiteListLabel=new QLabel("Display whitelist file and add a new mac to whitelist.");
    this->whiteListLabel->setVisible(false);
    this->setLabel=new QLabel("Set server's IP address, port number and operation mode");
    this->setLabel->setVisible(false);
    this->userLabel=new QLabel("Display online users' and total users' information and you can add a new user.");
    this->userLabel->setVisible(false);
    this->onlineUserLabel=new QLabel("Online users' information:");
    this->onlineUserLabel->setVisible(false);
    this->totalUserLabel=new QLabel("Total users' information:");
    this->totalUserLabel->setVisible(false);

    this->usernameLabel=new QLabel("Username:");
    this->usernameLabel->setVisible(false);
    this->usernameLine=new QLineEdit;
    this->usernameLine->setVisible(false);
    this->passwordLabel=new QLabel("Password:");
    this->passwordLabel->setVisible(false);
    this->passwordLine=new QLineEdit;
    this->passwordLine->setVisible(false);
    this->passwordLine->setEchoMode(QLineEdit::Password);


    connect(this->setButton, SIGNAL(clicked()), this, SLOT(setMode()));
    connect(this->exitButton, SIGNAL(clicked()), this, SLOT(exitServer()));
    connect(this->submitButton, SIGNAL(clicked()), this, SLOT(setModeOk()));
    connect(this->logButton, SIGNAL(clicked()), this, SLOT(showLog()));
    connect(this->hideLogButton, SIGNAL(clicked()), this, SLOT(hideLog()));
    connect(this->startButton, SIGNAL(clicked()), this, SLOT(startServer()));
    connect(this->stopButton, SIGNAL(clicked()), this, SLOT(stopServer()));
    connect(this->whitelistButton, SIGNAL(clicked()), this, SLOT(showWhitelist()));
    connect(this->hideWhitelistButton, SIGNAL(clicked()), this, SLOT(hideWhitelist()));
    connect(this->addMacButton, SIGNAL(clicked()), this, SLOT(addNewMac()));
    connect(this->aboutButton, SIGNAL(clicked()), this, SLOT(showAbout()));
    connect(this->hideSetButton, SIGNAL(clicked()), this, SLOT(hideSet()));
    connect(this->userInfoButton, SIGNAL(clicked()), this, SLOT(showUserInfo()));
    connect(this->hideUserInfoButton, SIGNAL(clicked()), this, SLOT(hideUserInfo()));
    connect(this->addUserButton, SIGNAL(clicked()), this, SLOT(addNewUser()));
    connect(this->deleteUserButton, SIGNAL(clicked()), this, SLOT(deleteUser()));
    connect(this->deleteMacButton, SIGNAL(clicked()),this, SLOT(deleteMac()));
    connect(this->userLogButton, SIGNAL(clicked()), this, SLOT(showUserLog()));
    connect(this->serverLogButton, SIGNAL(clicked()), this, SLOT(showServerLog()));
    connect(this->whitelistLogButton, SIGNAL(clicked()), this, SLOT(showWhitelistLog()));


    //The check box for set
    this->isBroadcastWhitelist=new QCheckBox(tr("&Don't broadcast the whitelist by server"));
    this->isBroadcastWhitelist->setVisible(false);
    this->encryptModeBox=new QComboBox;
    this->encryptModeBox->setVisible(false);
    this->encryptModeBox->setFixedSize(120, 30);
    this->encryptModeBox->addItem("CBC", NULL);
    this->encryptModeBox->addItem("ECB", NULL);

    //The text file for log
    this->logFile=new QTextEdit;
    this->logFile->setVisible(false);
    this->logFile->setReadOnly(true);

    //The text file for server listening
    this->listeningFile=new QTextEdit;
    this->listeningFile->setReadOnly(true);
    connect(this->listeningFile, SIGNAL(textChanged()), this, SLOT(moveEndLisFile()));


    //The text file for whitelist
    this->whitelistFile=new QTextEdit;
    this->whitelistFile->setVisible(false);
    this->whitelistFile->setReadOnly(true);

    //The text file for user information
    this->onlineUserFile=new QTextEdit;
    this->onlineUserFile->setVisible(false);
    this->onlineUserFile->setReadOnly(true);
    this->onlineUserFile->setLineWrapMode(QTextEdit::NoWrap);
    this->totalUserFile=new QTextEdit;
    this->totalUserFile->setVisible(false);
    this->totalUserFile->setReadOnly(true);
    this->totalUserFile->setLineWrapMode(QTextEdit::NoWrap);
    this->totalUserFile->setAutoFormatting(QTextEdit::AutoBulletList);

    QHBoxLayout *topLayout=new QHBoxLayout; //All the buttons is in one horizontal layout
    topLayout->addWidget(this->startButton);
    topLayout->addWidget(this->stopButton);
    topLayout->addWidget(this->setButton);
    topLayout->addWidget(this->logButton);
    topLayout->addWidget(this->whitelistButton);
    topLayout->addWidget(this->userInfoButton);
    topLayout->addWidget(this->aboutButton);
    topLayout->addWidget(this->exitButton);

    QHBoxLayout *ipLayout=new QHBoxLayout;
    ipLayout->addWidget(this->ipAddress);
    ipLayout->addWidget(this->ipLine);

    QHBoxLayout *portLayout=new QHBoxLayout;
    portLayout->addWidget(this->portNumber);
    portLayout->addWidget(this->portLine);

    QHBoxLayout *submitLayout=new QHBoxLayout;
    submitLayout->addWidget(this->hideSetButton);
    submitLayout->addStretch();
    submitLayout->addWidget(this->setLabel);
    submitLayout->addStretch();
    submitLayout->addWidget(this->submitButton);

    QHBoxLayout *boxLayout=new QHBoxLayout;
    boxLayout->addWidget(this->isBroadcastWhitelist);
    boxLayout->addStretch();
    boxLayout->addWidget(this->encryptModeBox);
    boxLayout->addStretch();

    QVBoxLayout *setLayout=new QVBoxLayout;  //The set layout
    setLayout->addLayout(ipLayout);
    setLayout->addLayout(portLayout);
    setLayout->addLayout(boxLayout);
    setLayout->addLayout(submitLayout);

    QHBoxLayout *logLayout=new QHBoxLayout;
    logLayout->addWidget(this->hideLogButton);
    logLayout->addStretch();
    logLayout->addWidget(this->logLabel);
    logLayout->addStretch();
    logLayout->addWidget(this->serverLogButton);
    logLayout->addWidget(this->userLogButton);
    logLayout->addWidget(this->whitelistLogButton);

    QVBoxLayout *allLogLayout=new QVBoxLayout;   //The layout of logfile with a hide button
    allLogLayout->addWidget(this->logFile);
    allLogLayout->addLayout(logLayout);

    QHBoxLayout *macLayout=new QHBoxLayout;
    macLayout->addWidget(this->macAddress);
    macLayout->addWidget(this->macLine);
    macLayout->addSpacing(20);
    macLayout->addWidget(this->sampleMacLabel);

    QHBoxLayout *addMacLayout=new QHBoxLayout;
    addMacLayout->addWidget(this->hideWhitelistButton);
    addMacLayout->addStretch();
    addMacLayout->addWidget(this->whiteListLabel);
    addMacLayout->addStretch();
    addMacLayout->addWidget(this->addMacButton);
    addMacLayout->addWidget(this->deleteMacButton);

    QVBoxLayout *whitelistLayout=new QVBoxLayout;  //The whitelist layout with the function of adding new mac address to whitelist
    whitelistLayout->addWidget(this->whitelistFile);
    whitelistLayout->addLayout(macLayout);
    whitelistLayout->addLayout(addMacLayout);

    QHBoxLayout *hideUserLayout=new QHBoxLayout;
    hideUserLayout->addWidget(this->hideUserInfoButton);
    hideUserLayout->addStretch();
    hideUserLayout->addWidget(this->userLabel);
    hideUserLayout->addStretch();
    hideUserLayout->addWidget(this->addUserButton);
    hideUserLayout->addWidget(this->deleteUserButton);

    QHBoxLayout *addUserLayout=new QHBoxLayout;
    addUserLayout->addWidget(this->usernameLabel);
    addUserLayout->addWidget(this->usernameLine);
    addUserLayout->addStretch();
    addUserLayout->addWidget(this->passwordLabel);
    addUserLayout->addWidget(this->passwordLine);

    QVBoxLayout *userInfoLayout=new QVBoxLayout;
    userInfoLayout->addWidget(this->onlineUserLabel);
    userInfoLayout->addWidget(this->onlineUserFile);
    userInfoLayout->addWidget(this->totalUserLabel);
    userInfoLayout->addWidget(this->totalUserFile);
    userInfoLayout->addLayout(addUserLayout);
    userInfoLayout->addLayout(hideUserLayout);

    QVBoxLayout *middleLayout=new QVBoxLayout;
    middleLayout->addWidget(this->listeningFile);  //Add listening file
    middleLayout->addLayout(setLayout);  //Add set layout
    middleLayout->addLayout(allLogLayout);  //Add log layout
    middleLayout->addLayout(whitelistLayout); //Add whitelist layout
    middleLayout->addLayout(userInfoLayout);

    QVBoxLayout *mainLayout=new QVBoxLayout;
    mainLayout->addLayout(topLayout); //Add all the buttons
    mainLayout->addLayout(middleLayout);

    this->setLayout(mainLayout);
}

Dialog::~Dialog()
{

}

void Dialog::showUserInfo()
{
    this->hideSet();
    this->hideWhitelist();
    this->hideLog();

    this->listeningFile->setVisible(false);

    this->onlineUserLabel->setVisible(true);
    this->onlineUserFile->setVisible(true);
    this->totalUserLabel->setVisible(true);
    this->totalUserFile->setVisible(true);
    this->userLabel->setVisible(true);
    this->hideUserInfoButton->setVisible(true);
    this->addUserButton->setVisible(true);
    this->deleteUserButton->setVisible(true);
    this->usernameLabel->setVisible(true);
    this->usernameLine->setVisible(true);
    this->passwordLabel->setVisible(true);
    this->passwordLine->setVisible(true);

    this->totalUserFile->clear();
    this->onlineUserFile->clear();
    this->usernameLine->clear();
    this->passwordLine->clear();

    KUsrInfo kui;
    kui.Initialize();
    TCHDB *hdb=(TCHDB *)kui.getDb(); //Connect the database to display all the user information
    struct userInfo *newUserInfo;
    tchdbiterinit(hdb);
    char *key=tchdbiternext2(hdb);
    int number=0;
    QString s;
    s.sprintf("<font color='blue'>Username:::::::::::::::::::::::::Flag:::::::::::::::::::::::::Last login time:::::::::::::::::::::::::Last logout time:::::::::::::::::::::::::Total login count:::::::::::::::::::::::::Total online time</font>");
    this->totalUserFile->append(s);
    this->onlineUserFile->append(s);
    while(key!=NULL)
    {
        int ksize = -1;
        newUserInfo = (struct userInfo*)tchdbget(hdb, key, strlen(key) + 1, &ksize);
        QString s;
        s.sprintf("%7s %28d %35s %28s %25d %42d", key, newUserInfo->flag, newUserInfo->lastLoginTime, newUserInfo->lastLogoutTime, newUserInfo->totalLoginCount, newUserInfo->totalOnlineTime);
        this->totalUserFile->append(s);
        if(newUserInfo->flag==1)//This user is online
        {
            ++number; //Increment the online user number
            this->onlineUserFile->append(s);
        }
        key=tchdbiternext2(hdb);
    }
    s.clear();
    s.sprintf("<font color='red'>There are %d users now.</font>", tchdbrnum(hdb));
    this->totalUserFile->append(s);
    s.clear();
    s.sprintf("<font color='red'>Tere are %d users online now.</font>", number);
    this->onlineUserFile->append(s);

    this->userLogFile.WriteLog(LEVEL_INFO, "Look up user database.  Total online user number is %d. Total user number is %d.", number, tchdbrnum(hdb)); //Writelog
}

void Dialog::deleteUser()
{
    if(!(this->usernameLine->text().length()<3))
    {
        KUsrInfo kui;
        kui.Initialize();
        TCHDB *hdb=(TCHDB *)kui.getDb(); //Connect the database to display all the user information

        char username[g_ciMaxUserNameLen];
        int l=this->usernameLine->text().length();
        int length=l<g_ciMaxUserNameLen-1 ? l : g_ciMaxUserNameLen-1;
        strncpy(username, this->usernameLine->text().toLatin1(), length);
        username[length]='\0';

        int ksize = -1;
        if(((struct userInfo*)tchdbget(hdb, username, strlen(username) + 1, &ksize))==NULL ) //User not found, so return 1
        {
            QMessageBox::warning(this, "Delete a user", "User not found!");
        }else
        {
            this->listeningFile->setVisible(true);

            this->onlineUserLabel->setVisible(false);
            this->onlineUserFile->setVisible(false);
            this->totalUserLabel->setVisible(false);
            this->totalUserFile->setVisible(false);
            this->hideUserInfoButton->setVisible(false);
            this->userLabel->setVisible(false);
            this->addUserButton->setVisible(false);
            this->deleteUserButton->setVisible(false);
            this->usernameLabel->setVisible(false);
            this->usernameLine->setVisible(false);
            this->passwordLabel->setVisible(false);
            this->passwordLine->setVisible(false);

            if (!tchdbout(hdb, username, strlen(username) + 1)) //Delete a user
            {
                int ecode;
                ecode = tchdbecode(hdb);
                QString s;
                s.sprintf("<font color='blue'>Delete user ");
                s.append(this->usernameLine->text().toLatin1());
                s.append(" failure!</font> Reason:");
                s.append(tchdberrmsg(ecode));
                this->listeningFile->append(s);

                this->userLogFile.WriteLog(LEVEL_ERROR, "Delete user %s failure!", this->usernameLine->text().toStdString().c_str()); //Write user log file
            }else
            {
                QString s;
                s.sprintf("<font color='blue'>Delete user ");
                s.append(this->usernameLine->text().toLatin1());
                s.append(" successful!</font>");
                this->listeningFile->append(s);

                this->userLogFile.WriteLog(LEVEL_INFO, "Delete user %s successful!", this->usernameLine->text().toStdString().c_str());
            }
        }
    }else
    {
        QMessageBox::warning(this, "Delete a user", "Please input username!");

    }
}

void Dialog::addNewUser()
{
    if(!(this->usernameLine->text().length()<3 || this->passwordLine->text().length()<3))
    {//If you input an username and a password
        KUsrInfo kui;
        kui.Initialize();
        TCHDB *hdb=(TCHDB *)kui.getDb(); //Connect the database to display all the user information
        struct userInfo newUserInfo;
        char username[g_ciMaxUserNameLen];
        int l=this->usernameLine->text().length();
        int length=l<g_ciMaxUserNameLen-1 ? l : g_ciMaxUserNameLen-1;
        strncpy(username, this->usernameLine->text().toLatin1(), length);
        username[length]='\0';
        int ksize=-1;
        if(((struct userInfo*)tchdbget(hdb, username, strlen(username) + 1, &ksize))==NULL)
        {//If you want to add a new user and the username is not exist, you can add it
            this->listeningFile->setVisible(true);

            this->onlineUserLabel->setVisible(false);
            this->onlineUserFile->setVisible(false);
            this->totalUserLabel->setVisible(false);
            this->totalUserFile->setVisible(false);
            this->hideUserInfoButton->setVisible(false);
            this->userLabel->setVisible(false);
            this->addUserButton->setVisible(false);
            this->deleteUserButton->setVisible(false);
            this->usernameLabel->setVisible(false);
            this->usernameLine->setVisible(false);
            this->passwordLabel->setVisible(false);
            this->passwordLine->setVisible(false);

            l=this->passwordLine->text().length();
            length=l<g_ciMaxPwdLen-1 ? l : g_ciMaxPwdLen-1;
            strncpy(newUserInfo.ccArrPwd, this->passwordLine->text().toLatin1(), length);
            newUserInfo.ccArrPwd[length]='\0';
            newUserInfo.flag=0;
            newUserInfo.lastLoginTime[0]='0';
            newUserInfo.lastLoginTime[1]='\0';
            newUserInfo.lastLogoutTime[0]='0';
            newUserInfo.lastLogoutTime[1]='\0';
            newUserInfo.totalLoginCount=0;
            newUserInfo.totalOnlineTime=0;
            if (!tchdbput(hdb, username, strlen(username) + 1,
                &newUserInfo, sizeof(struct userInfo))) //Update user information
            {
                int ecode;
                ecode = tchdbecode(hdb);
                QString s;
                s.sprintf("<font color='blue'>Add new user ");
                s.append(this->usernameLine->text().toLatin1());
                s.append(" failure!</font> Reason:");
                s.append(tchdberrmsg(ecode));
                this->listeningFile->append(s);

                this->userLogFile.WriteLog(LEVEL_ERROR, "Add new user %s failure!", this->usernameLine->text().toStdString().c_str());
            }else
            {
                QString s;
                s.sprintf("<font color='blue'>Add new user ");
                s.append(this->usernameLine->text().toLatin1());
                s.append(" successful!</font>");
                this->listeningFile->append(s);

                this->userLogFile.WriteLog(LEVEL_INFO, "Add new user %s successful!", this->usernameLine->text().toStdString().c_str());
            }
        }else
        {
            QMessageBox::warning(this, "Add a new user", "The username is in the database!");

        }
    }else
    {
        QMessageBox::warning(this, "Add a new user", "Please input username and password!");

    }
}

void Dialog::hideUserInfo()
{
    this->listeningFile->setVisible(true);

    this->onlineUserLabel->setVisible(false);
    this->onlineUserFile->setVisible(false);
    this->totalUserLabel->setVisible(false);
    this->totalUserFile->setVisible(false);
    this->hideUserInfoButton->setVisible(false);
    this->userLabel->setVisible(false);
    this->addUserButton->setVisible(false);
    this->deleteUserButton->setVisible(false);
    this->usernameLabel->setVisible(false);
    this->usernameLine->setVisible(false);
    this->passwordLabel->setVisible(false);
    this->passwordLine->setVisible(false);
}

void Dialog::showWhitelist()
{
    this->hideSet();
    this->hideLog();
    this->hideUserInfo();

    this->listeningFile->setVisible(false);
    this->whitelistFile->setVisible(true);
    this->macAddress->setVisible(true);
    this->macLine->setVisible(true);
    this->sampleMacLabel->setVisible(true);
    this->hideWhitelistButton->setVisible(true);
    this->whiteListLabel->setVisible(true);
    this->addMacButton->setVisible(true);
    this->deleteMacButton->setVisible(true);

    this->whitelistFile->clear();
    this->macLine->clear();

    vector<KMac> whitelist=newServer.getKWhitelist().getWhitelist();
    int count=whitelist.size();
    int i=0;
    while(i<count)
    {
        QString mac;
        mac.sprintf("%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x", whitelist[i].GetAddr()[0], whitelist[i].GetAddr()[1], whitelist[i].GetAddr()[2], whitelist[i].GetAddr()[3], whitelist[i].GetAddr()[4], whitelist[i].GetAddr()[5], whitelist[i].GetAddr()[6], whitelist[i].GetAddr()[7]);
        this->whitelistFile->append(mac);
        ++i;
    }
    QString s;
    s.sprintf("<font color='red'>There are total %d MACs in the whitelist.</font>", count);
    this->whitelistFile->append(s);

    this->whitelistLogFile.WriteLog(LEVEL_INFO, "Look up whitelist information. There are total %d MACs in the whitelist.", count);
}

void Dialog::hideWhitelist()
{
    this->listeningFile->setVisible(true);
    this->whitelistFile->setVisible(false);
    this->macAddress->setVisible(false);
    this->macLine->setVisible(false);
    this->sampleMacLabel->setVisible(false);
    this->hideWhitelistButton->setVisible(false);
    this->whiteListLabel->setVisible(false);
    this->addMacButton->setVisible(false);
    this->deleteMacButton->setVisible(false);

}

void Dialog::addNewMac()
{
    if(this->macLine->text().isEmpty() || this->macLine->text().length()!=17)
    {
        QMessageBox::warning(this, "Add a new MAC", "Please input a correct new MAC!");
        return;
    }

    QString s=this->macLine->text(); //The MAC that you input
    s=s.toUpper();
    unsigned char newMac[g_ciMaxMacAddrLen]={0x00};
    if(!(this->string_to_unsignedchar(s.toStdString().c_str(), newMac))) //If return false, this indicates the input has error
    {
        QMessageBox::warning(this, "Add a new MAC", "Please input a correct new MAC!");
        return;
    }

    KMac kmac;
    kmac.SetAddr(newMac);
    if(newServer.addWhitelist(kmac))//Add MAC successfully
    {
        QString sm;
        if(this->startOrNot==START) //If the server has been started, we should issue the new MAC to other clients
        {
            struct clientInfo addNewMac;
            addNewMac.dialog=this;
            vector<int> vcs=newServer.getClientSocket();
            addNewMac.vClientSockfds=&vcs;
            addNewMac.kpProto.m_kmMac=kmac; //The new MAC
            newServer.notifyAllClients(ADD, &addNewMac);

            sm.sprintf("<font color='blue'>Add! Has notified all other %d clients.</font>", vcs.size());
            this->listeningFile->append(sm);
            this->whitelistLogFile.WriteLog(LEVEL_INFO, "Add! Has notified all other %d clients.", vcs.size());
        }
        sm.clear();
        sm.sprintf("Add a new MAC to whitelist, it is : %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x", newMac[0], newMac[1], newMac[2], newMac[3], newMac[4], newMac[5], newMac[6], newMac[7]);
        this->listeningFile->append(sm);
        this->whitelistLogFile.WriteLog(LEVEL_INFO, sm.toStdString().c_str());

        this->listeningFile->setVisible(true);
        this->whitelistFile->setVisible(false);
        this->macAddress->setVisible(false);
        this->macLine->setVisible(false);
        this->hideWhitelistButton->setVisible(false);
        this->whiteListLabel->setVisible(false);
        this->addMacButton->setVisible(false);
        this->deleteMacButton->setVisible(false);
    }else
    {
        QMessageBox::warning(this, "Add a new MAC", "The MAC is already in the whitelist! Please input again!");
    }
}

void Dialog::deleteMac()
{
    if(this->macLine->text().isEmpty())
    {
        QMessageBox::warning(this, "Delete a MAC", "Please input a MAC from the whitelist!");
        return;
    }

    QString s=this->macLine->text(); //The MAC that you input
    s=s.toUpper();
    unsigned char newMac[g_ciMaxMacAddrLen]={0x00};
    if(!(this->string_to_unsignedchar(s.toStdString().c_str(), newMac))) //If return false, this indicates the input has error
    {
        QMessageBox::warning(NULL, "Add a new MAC", "Please input a correct new MAC!");
        return;
    }

    KMac kmac;
    kmac.SetAddr(newMac);
    QString sm;
    if(this->startOrNot==START) //If the server has been started, we should issue the new MAC to other clients
    {
        struct clientInfo addNewMac;
        addNewMac.dialog=this;
        vector<int> vcs=newServer.getClientSocket();
        addNewMac.vClientSockfds=&vcs;
        addNewMac.kpProto.m_kmMac=kmac; //The new MAC
        newServer.notifyAllClients(DEL, &addNewMac);

        sm.sprintf("<font color='blue'>Delete! Has notified all other %d clients.</font>", vcs.size());
        this->listeningFile->append(sm);
        this->whitelistLogFile.WriteLog(LEVEL_INFO, "Delete! Has notified all other %d clients.", vcs.size());
    }
    if(newServer.deleteWhitelist(kmac)) //Delete successful
    {
        sm.clear();
        sm.sprintf("Delete a MAC from whitelist, it is : %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x", newMac[0], newMac[1], newMac[2], newMac[3], newMac[4], newMac[5], newMac[6], newMac[7]);
        this->listeningFile->append(sm);
        this->whitelistLogFile.WriteLog(LEVEL_INFO, sm.toStdString().c_str());

        this->listeningFile->setVisible(true);
        this->whitelistFile->setVisible(false);
        this->macAddress->setVisible(false);
        this->macLine->setVisible(false);
        this->hideWhitelistButton->setVisible(false);
        this->whiteListLabel->setVisible(false);
        this->addMacButton->setVisible(false);
        this->deleteMacButton->setVisible(false);
    }else //Attempt to delete a not exist MAC
    {
        QMessageBox::warning(this, "Delete a MAC", "The MAC is not in the whitelist! Please input again!");
    }
}

void Dialog::setMode()
{
    this->hideLog();
    this->hideWhitelist();
    this->hideUserInfo();

    this->listeningFile->setVisible(false);
    this->isBroadcastWhitelist->setVisible(true);
    this->encryptModeBox->setVisible(true);;
    this->submitButton->setVisible(true);
    this->setLabel->setVisible(true);
    this->hideSetButton->setVisible(true);
    this->ipAddress->setVisible(true);
    this->portNumber->setVisible(true);
    this->ipLine->setVisible(true);
    this->portLine->setVisible(true);

    this->ipLine->clear();
    this->portLine->clear();
}

void Dialog::setModeOk()
{
    this->listeningFile->setVisible(true);
    this->isBroadcastWhitelist->setVisible(false);
    this->encryptModeBox->setVisible(false);
    this->submitButton->setVisible(false);
    this->setLabel->setVisible(false);
    this->hideSetButton->setVisible(false);
    this->ipAddress->setVisible(false);
    this->portNumber->setVisible(false);
    this->ipLine->setVisible(false);
    this->portLine->setVisible(false);

    if(this->isBroadcastWhitelist->isChecked())
    {
        this->whitelisIssuePolicy=REQUEST; //Indicate the server's will not broadcast the new MAC to all the clients, it will only broadcast a new MAC to all the application server
        this->serverLogFile.WriteLog(LEVEL_INFO, "Set whitelist issue policy to client-request!");
    }else
    {
        this->whitelisIssuePolicy=BROADCAST;
        this->serverLogFile.WriteLog(LEVEL_INFO, "Set whitelist issue policy to server-broadcast!");
    }

    if(this->encryptModeBox->currentText()=="CBC")
    {
        this->serverEncryptMode=CBC;
        this->serverLogFile.WriteLog(LEVEL_INFO, "Set encrypt mode to CBC!");
    }else
    {
        this->serverEncryptMode=ECB;
        this->serverLogFile.WriteLog(LEVEL_INFO, "Set encrypt mode to ECB!");
    }

    if(!(this->ipLine->text().isEmpty())) //Set IP address
    {
        int l=this->ipLine->text().length();
        int length=l<g_ciAddrStrLen-1  ? l : g_ciAddrStrLen-1;
        strncpy(this->ccArrLocalAddr, (this->ipLine->text()).toLatin1(), length);
        this->ccArrLocalAddr[length]='\0';
        this->serverLogFile.WriteLog(LEVEL_INFO, "Set server's IP address to %s.", this->ccArrLocalAddr);
    }
    if(!(this->portLine->text().isEmpty())) //Set port number
    {
        this->ciPort=this->portLine->text().toInt();
        this->serverLogFile.WriteLog(LEVEL_INFO, "Set server's port number to %d.", this->ciPort);
    }
}

void Dialog::hideSet()
{
    this->listeningFile->setVisible(true);
    this->isBroadcastWhitelist->setVisible(false);
    this->submitButton->setVisible(false);
    this->encryptModeBox->setVisible(false);
    this->setLabel->setVisible(false);
    this->hideSetButton->setVisible(false);
    this->ipAddress->setVisible(false);
    this->portNumber->setVisible(false);
    this->ipLine->setVisible(false);
    this->portLine->setVisible(false);
}

void Dialog::showLog()
{
    this->hideSet();
    this->hideWhitelist();
    this->hideUserInfo();

    this->listeningFile->setVisible(false);
    this->logFile->setVisible(true);
    this->logLabel->setVisible(true);
    this->hideLogButton->setVisible(true);
    this->serverLogButton->setVisible(true);
    this->userLogButton->setVisible(true);
    this->whitelistLogButton->setVisible(true);
}

void Dialog::showUserLog()
{
    this->logFile->clear();
    ifstream file("log/userLogFile");
    if(!file.is_open()) //If open the file fail
    {
        this->serverLogFile.WriteLog(LEVEL_ERROR, "Open the userLogFile fail!");
        this->logFile->append("<font color='red'>Open the userLogFile fail!</font>");
    }else
    {
        //this->serverLogFile.WriteLog(LEVEL_INFO, "Look up user log file");
        QString qs;
        while(!file.eof())
        {
            string s;
            getline(file, s);
            s.append("\n");
            qs.prepend(s.c_str());
        }
        this->logFile->append(qs.toStdString().c_str());
    }
    file.close();
}

void Dialog::showServerLog()
{
    this->logFile->clear();
    ifstream file("log/serverLogFile");
    if(!file.is_open()) //If open the file fail
    {
        this->serverLogFile.WriteLog(LEVEL_ERROR, "Open the serverLogFile fail!");
        this->logFile->append("<font color='red'>Open the serverLogFile fail!</font>");
    }else
    {
        //this->serverLogFile.WriteLog(LEVEL_INFO, "Look up server log file");
        QString qs;
        while(!file.eof())
        {
            string s;
            getline(file, s);
            s.append("\n");
            qs.prepend(s.c_str());
        }
        this->logFile->append(qs.toStdString().c_str());
    }
    file.close();
}

void Dialog::showWhitelistLog()
{
    this->logFile->clear();
    ifstream file("log/whitelistLogFile");
    if(!file.is_open()) //If open the file fail
    {
        this->serverLogFile.WriteLog(LEVEL_ERROR, "Open the whitelistLogFile fail!");
        this->logFile->append("<font color='red'>Open the whitelistLogFile fail!</font>");
    }else
    {
        //this->serverLogFile.WriteLog(LEVEL_INFO, "Look up whitelist log file");
        QString qs;
        while(!file.eof())
        {
            string s;
            getline(file, s);
            s.append("\n");
            qs.prepend(s.c_str());
        }
        this->logFile->append(qs.toStdString().c_str());
    }
    file.close();
}

void Dialog::hideLog()
{
    this->logFile->clear();

    this->listeningFile->setVisible(true);
    this->logFile->setVisible(false);
    this->logLabel->setVisible(false);
    this->hideLogButton->setVisible(false);
    this->serverLogButton->setVisible(false);
    this->userLogButton->setVisible(false);
    this->whitelistLogButton->setVisible(false);
}

void *startNow(void *dialog)
{
    newServer.startServer(dialog);
    pthread_exit(NULL);
}


void Dialog::startServer()
{
    if(newServer.getRsaKeyN()==0)  //Rsa has not been set, so server can not be started
    {
        QMessageBox::warning(this, "NacServer", "The server is in configuration, pleas wait for a moment. Thank you!", QMessageBox::Ok, 0);
        return;
    }

    newServer.InitWhitelist();

    this->startOrNot=START; //Indicate the server has been started
    this->listeningFile->clear();

    this->hideSet();
    this->hideWhitelist();
    this->hideLog();
    this->hideUserInfo();

    this->startButton->setEnabled(false);
    this->stopButton->setEnabled(true);
    this->setButton->setEnabled(false);
    this->exitButton->setEnabled(false);
    this->submitButton->setEnabled(false);

    KUsrInfo kui;
    kui.Initialize();
    TCHDB *hdb=(TCHDB *)kui.getDb(); //First, connect the database to offline all the online users
    struct userInfo *newUserInfo;
    tchdbiterinit(hdb);
    char *key=tchdbiternext2(hdb);
    while(key!=NULL)
    {
        int ksize = -1;
        char username[g_ciMaxUserNameLen];
        strncpy(username, key, strlen(key));
        username[strlen(key)]='\0';
        newUserInfo = (struct userInfo*)tchdbget(hdb, username, strlen(username) + 1, &ksize);
        if(newUserInfo->flag==1)//This user is online
        {
            newUserInfo->flag=0; //All online users are forcing quit
            time_t t;
            t=time(NULL);
            char *tt=ctime(&t);
            strncpy(newUserInfo->lastLogoutTime, tt, strlen(tt)); //Record this login time
            newUserInfo->lastLogoutTime[strlen(tt)-1]='\0';
            if (!tchdbput(hdb, username, strlen(username) + 1,
                        newUserInfo, sizeof(struct userInfo))) //Update user information
            {
                int ecode;
                ecode = tchdbecode(hdb);
                QString ms;
                ms.sprintf("Start server failure: %d!", ecode);
                this->listeningFile->append(ms);
                this->serverLogFile.WriteLog(LEVEL_ERROR, ms.toStdString().c_str());
                return;
            }
        }
        key=tchdbiternext2(hdb);
    }

    time_t t;
    t=time(NULL);
    QString startInfo;
    startInfo.sprintf("<font color='red'>Start server!</font>:::%s", ctime(&t));
    this->listeningFile->append(startInfo);
    this->serverLogFile.WriteLog(LEVEL_INFO, "Start server!");

    QString s;
    s.sprintf("Set RSA key N to %lld.", newServer.getRsaKeyN());
    this->listeningFile->append(s);
    this->serverLogFile.WriteLog(LEVEL_INFO, s.toStdString().c_str());
    s.clear();
    s.sprintf("Set RSA key E to %lld.", newServer.getRsaKeyE());
    this->listeningFile->append(s);
    this->serverLogFile.WriteLog(LEVEL_INFO, s.toStdString().c_str());

    pthread_attr_t attr;  //The attribute of the thread
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&pt, &attr, &startNow, (void *)this);
    pthread_attr_destroy(&attr);
}

void *stopNow(void *)
{
    newServer.stopServer();
    pthread_exit(NULL);

}

void Dialog::stopServer()
{
    int yesOrNo=QMessageBox::question(this, "Question", "Are you sure to stop server?", QMessageBox::Yes, QMessageBox::No);
    if(yesOrNo==QMessageBox::Yes)
    {
        newServer.clearWhitelist(); //Stop the server and clear the whitelist

        this->startOrNot=STOP; //Indicate the server has been stopped

        this->hideSet();
        this->hideWhitelist();
        this->hideLog();

        this->startButton->setEnabled(true);
        this->stopButton->setEnabled(false);
        this->setButton->setEnabled(true);
        this->exitButton->setEnabled(true);
        this->submitButton->setEnabled(true);

        time_t t;
        t=time(NULL);
        QString startInfo;
        startInfo.sprintf("<font color='red'>Stop server!</font>:::%s", ctime(&t));
        this->listeningFile->append(startInfo);
        this->listeningFile->append("");
        this->serverLogFile.WriteLog(LEVEL_INFO, "Stop server!");

        pthread_cancel(pt); //Shutdown the server
        pthread_attr_t attr;  //The attribute of the thread
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        pthread_create(&pt, &attr, &stopNow, NULL);
        pthread_attr_destroy(&attr);
    }
}

void Dialog::exitServer()
{
    this->close();
}

void Dialog::showAbout()
{
    QMessageBox::about(this, "About NacClient", "NacServer 1.0.0\nBased on Linux & C++ & Qt 4.7.0\nBuilt on May 20 2013\nCopyright 2013-2013 Nac Corporation. All rights reserved.\nAuthor: Qin Peixi, Hao Zhanzhu, Zhang Yaolin");
}

QTextEdit* Dialog::getLisFile()
{
    return this->listeningFile;
}

char* Dialog::getIpAddr()
{
    return this->ccArrLocalAddr;
}

int Dialog::getPort()
{
    return this->ciPort;
}

enum wlIssuePolicy Dialog::getWlIssuePolicy()
{
    return this->whitelisIssuePolicy;
}

void Dialog::setWlIssuePolicy(enum wlIssuePolicy policy)
{
    this->whitelisIssuePolicy=policy;
}

char digit_convert(char ch)
{
    return (ch-'0');
}

char char_convert(char ch)
{
    return (ch-'A'+10);
}

bool Dialog::string_to_unsignedchar(const char *mac,unsigned char newMac[])
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


CMyLog Dialog::getUserLogFile()
{
    return this->userLogFile;
}

CMyLog Dialog::getServerLogFile()
{
    return this->serverLogFile;
}

CMyLog Dialog::getWhitelistLogFile()
{
    return this->whitelistLogFile;
}

void Dialog::serverWriteLog(LEVEL_TYPE logLevel, const char* format,...)
{
    this->serverLogFile.WriteLog(logLevel, format);
}

void Dialog::appendLisFile(QString s)
{
    this->listeningFile->append(s);
}

enum encryptMode Dialog::getEnMode()
{
    return this->serverEncryptMode;
}

void Dialog::closeEvent( QCloseEvent* qce )
{
    int yesOrNo=QMessageBox::question(this, "NacClient", "Are you sure to exit?", QMessageBox::Yes, QMessageBox::No);
    if(yesOrNo == QMessageBox::Yes)
    {
        this->serverLogFile.WriteLog(LEVEL_INFO, "Exit server!");
        qce->accept();
    }else
    {
        qce->ignore();
    }
}

void Dialog::moveEndLisFile()
{
    QTextCursor cur=this->listeningFile->textCursor();
    cur.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    this->listeningFile->setTextCursor(cur);

}

void Dialog::enableStartButton()
{
    this->startButton->setEnabled(true);
}

void Dialog::showRsaOk()
{
    while(newServer.getRsaKeyN()==0)
    {

    }
    QMessageBox::information(this, "NacSever", "Rsa has been set!", QMessageBox::Ok, 0);
}
