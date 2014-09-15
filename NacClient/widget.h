#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui>
#include "common/constants.h"

class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;
class QVBoxLayout;
class QTextBrowser;

enum logFlag
{
    SUCCESS, FAIL, HASLOGIN, REFUSED, OTHER, LOGOUT
};

enum wlIssuePolicy
{
    BROADCAST, REQUEST
};

enum encryptMode
{
    ECB, CBC
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    char *getUsername();
    char *getPassword();
    QTextEdit *getListeningFile();
    enum logFlag getFlag();
    void setFlag(enum logFlag);
    char *getIpAddress();
    int getPortNumber();
    void appendLisFile(QString s);
    void closeEvent( QCloseEvent* qce);
    bool string_to_unsignedchar(const char *mac,unsigned char newMac[]);

private slots:
    void login();
    void loginOK();
    void logout();
    void exitClient();
    void clearListeningFile();
    void showAbout();
    void setServer();
    void setServerOk();
    void hideSet();
    void showUsualModel();
    void returnToSetModel();
    void showFingerprintModel();
    void showPhoneModel();
    void moveEndLisFile();


signals:
    void loginSuccess();


private:
    QLabel *usernameLabel;
    QLabel *passwordLabel;
    QLabel *ipAddress;
    QLabel *portNumber;
    QLabel *macAddress;
    QLabel *setLabel;
    QLabel *modelLabel;
    QLabel *fingerprintModelLabel;
    QLabel *phoneModelLabel;

    QLineEdit *usernameLine;
    QLineEdit *passwordLine;
    QLineEdit *ipLine;
    QLineEdit *portLine;
    QLineEdit *macLine;

    QPushButton *loginButton;
    QPushButton *logoutButton;
    QPushButton *exitButton;
    QPushButton *aboutButton;
    QPushButton *setButton;
    QPushButton *setSubmitButton;
    QPushButton *hideSetButton;
    QPushButton *usualModelButton;
    QPushButton *fingerprintModelButton;
    QPushButton *phoneModelButton;
    QPushButton *returnToModelButton;
    QPushButton *returnToModelButton2;

    QTextEdit *listeningFile;
    //QTextBrowser *listeningFile;

    char username[g_ciMaxUserNameLen];
    char password[g_ciMaxPwdLen];
    char ccArrLocalAddr[g_ciAddrStrLen];

    int ciPort;
    enum logFlag flag;
    pthread_t pt;
};


#endif // WIDGET_H
