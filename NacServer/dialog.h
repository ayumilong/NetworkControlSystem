#ifndef DIALOG_H
#define DIALOG_H

#include <QtGui/qwidget.h>
#include <time.h>
#include <vector>
#include <fstream>
#include <iostream>

#include "common/constants.h"
#include "server/KUsrInfo.h"
#include "server/mylog.h"
#include "common/KMac.hpp"
#include "common/rsa.h"

class QPushButton;
class QCheckBox;
class QTextEdit;
class QLabel;
class QLineEdit;
class QComboBox;

enum serverStatus
{
    START, STOP
};

enum wlIssuePolicy
{
    BROADCAST, REQUEST
};

enum encryptMode
{
    ECB, CBC
};

class Dialog : public QWidget
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();
    QTextEdit *getLisFile();
    char *getIpAddr();
    int getPort();
    CMyLog getUserLogFile();
    CMyLog getServerLogFile();
    CMyLog getWhitelistLogFile();
    enum wlIssuePolicy getWlIssuePolicy();
    void setWlIssuePolicy(enum wlIssuePolicy policy);
    bool string_to_unsignedchar(const char *mac,unsigned char newMac[]);
    void serverWriteLog(LEVEL_TYPE logLevel, const char* format,...);
    void appendLisFile(QString s);
    enum encryptMode getEnMode();
    void enableStartButton();
    void showRsaOk();

private slots:
    void setMode();
    void setModeOk();
    void hideSet();

    void showLog();
    void showUserLog();
    void showServerLog();
    void showWhitelistLog();
    void hideLog();

    void showWhitelist();
    void addNewMac();
    void deleteMac();
    void hideWhitelist();

    void showUserInfo();
    void addNewUser();
    void deleteUser();
    void hideUserInfo();

    void startServer();
    void stopServer();
    void exitServer();

    void showAbout();
    void closeEvent( QCloseEvent* qce );
    void moveEndLisFile();

private:
    QPushButton *startButton;
    QPushButton *stopButton;
    QPushButton *setButton;
    QPushButton *logButton;
    QPushButton *userLogButton;
    QPushButton *serverLogButton;
    QPushButton *whitelistLogButton;
    QPushButton *whitelistButton;
    QPushButton *hideWhitelistButton;
    QPushButton *exitButton;
    QPushButton *addMacButton;
    QPushButton *deleteMacButton;
    QPushButton *submitButton;
    QPushButton *hideLogButton;
    QPushButton *hideSetButton;
    QPushButton *userInfoButton;
    QPushButton *hideUserInfoButton;
    QPushButton *aboutButton;
    QPushButton *addUserButton;
    QPushButton *deleteUserButton;

    QCheckBox *isBroadcastWhitelist;
    QComboBox *encryptModeBox;

    QTextEdit *logFile;
    QTextEdit *listeningFile;
    QTextEdit *whitelistFile;
    QTextEdit *onlineUserFile;
    QTextEdit *totalUserFile;

    QLabel *ipAddress;
    QLabel *portNumber;
    QLabel *macAddress;
    QLabel *logLabel;
    QLabel *whiteListLabel;
    QLabel *setLabel;
    QLabel *userLabel;
    QLabel *onlineUserLabel;
    QLabel *totalUserLabel;
    QLabel *usernameLabel;
    QLabel *passwordLabel;
    QLabel *sampleMacLabel;

    QLineEdit *ipLine;
    QLineEdit *portLine;
    QLineEdit *macLine;
    QLineEdit *usernameLine;
    QLineEdit *passwordLine;

    char ccArrLocalAddr[g_ciAddrStrLen]; //Server's IP address
    int ciPort;  //Server's port number
    enum serverStatus startOrNot; //Indicate server's status
    enum wlIssuePolicy whitelisIssuePolicy; //Indicate server's whitelist issue policy
    enum encryptMode serverEncryptMode;
    CMyLog userLogFile;
    CMyLog serverLogFile;
    CMyLog whitelistLogFile;
};

#endif // DIALOG_H
