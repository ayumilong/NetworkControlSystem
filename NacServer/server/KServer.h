#ifndef KSERVER_H
#define KSERVER_H

#include <iostream>
#include <vector>
#include <assert.h>
#include "KServerSocket.h"
#include "KUsrInfo.h"
#include "common/KProtocol.hpp"
#include "common/KWhiteList.h"
#include "common/KMac.hpp"
#include "common/DES.h"
#include "common/rsa.h"
#include "dialog.h"
#include <QtGui/qtextedit.h>
#include <Qt/qstring.h>
#include <server/mylog.h>

using namespace std;

struct clientInfo
{
    Dialog *dialog;
    KWhiteList *g_kwlWhitelist;
    KServerSocket *kssSocket;
    vector<int> *vClientSockfds;
    RSA *rsa;
    CMyLog *serverLog;

    unsigned long long rsaKeyN;
    unsigned long long rsaKeyE;
    int client_sock;
    KProtocol kpProto;
    char sMsgBuf[g_ciMaxMsgLen];
};

class KServer
{
public:
    KServer();
    void startServer(void *);
    void stopServer();
    void notifyAllClients(ECmd cmd, struct clientInfo* client);
    KWhiteList getKWhitelist();
    bool addWhitelist(const KMac &kmac);
    bool deleteWhitelist(const KMac &kmac);
    void clearWhitelist();
    vector<int> getClientSocket();
    unsigned long long getRsaKeyN();
    unsigned long long getRsaKeyE();
    void setRsaKeyN(unsigned long long keyN);
    void setRsaKeyE(unsigned long long keyE);
    void InitWhitelist();
    RSA getRsa();
    void generateRsaKey();

private:
    KWhiteList g_kwlWhitelist;
    KServerSocket kssSocket;
    vector<int> vClientSockfds;
    CMyLog serverLog;
    RSA rsa;
    unsigned long long rsaKeyN;
    unsigned long long rsaKeyE;
};

#endif // KSERVER_H
