#ifndef KCLIENT_H
#define KCLIENT_H

#include <pthread.h>
#include "client/KClientSocket.h"
#include "client/KFilter.h"
#include "common/KProtocol.hpp"
#include "common/KWhiteList.h"
#include "common/rsa.h"
#include "common/DES.h"
#include "widget.h"

class KFilter;

struct filterInfo
{
    KWhiteList *g_kwlWhitelist;
    Widget *widget;
};

class KClient
{
public:
    KClient();
    void startClient(void *);
    void keepClient(void *);
    void stopClient(void *);
    KFilter *getKfilter();
    void getLocalMac(unsigned char *mac);
    void InitWhitelist();
    void startProtection(Widget *widget);
    void stopProtection();
    void rsaAndDes(RSA rsa);
    KWhiteList getWhitelist();
    void addServerMac(KMac sMac);

private:    
    KWhiteList g_kwlWhitelist;
    KClientSocket kcsSock;
    KProtocol kpProto;
    KFilter *kfilter;
    CDES des;

    pthread_t pid;
    enum wlIssuePolicy whitelistIssuePolicy;
    enum encryptMode enMode;

    char sMsgBuf[g_ciMaxMsgLen];
    char desKey[DES_KEY_LEN];
};

#endif // KCLIENT_H
