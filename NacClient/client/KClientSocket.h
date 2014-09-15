/*************************************************************************/
/* Copyright (C) Network Accessing Control -USTC, 2013                   */
/*                                                                       */
/*  File Name              :  client/KClientSocket.h                     */
/*  Pricipal Author        :  qinpeixi                                   */
/*  Subsystem Name         :                                             */
/*  Module Name            :                                             */
/*  Language               :                                             */
/*  Target Environment     :                                             */
/*  Created Time           :  Sat 09 Mar 2013 05:21:12 PM CST            */
/*  Description            :                                             */
/*************************************************************************/

#ifndef KCLIENT_SOCKET_H
#define KCLIENT_SOCKET_H

#include "common/constants.h"

class KClientSocket
{
public:
    KClientSocket();
    KClientSocket(const KClientSocket &s);
    KClientSocket operator=(const KClientSocket &s);
    int OpenRemoteService(
            const char (&ccArrAddr)[g_ciAddrStrLen], 
            const int ciPort);
    void CloseRemoteService() const;
    void SendMsg(const char (&ccArrMsg)[g_ciMaxMsgLen]) const;
    int RecvMsg(char (&cArrMsg)[g_ciMaxMsgLen]) const;
    int getSockfd();
private:
    int m_iSockfd;
};

#endif
