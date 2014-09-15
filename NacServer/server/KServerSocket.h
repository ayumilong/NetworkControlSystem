/*************************************************************************/
/* Copyright (C) Network Accessing Control -USTC, 2013                   */
/*                                                                       */
/*  File Name              :  server/KServerSocket.h                      */
/*  Pricipal Author        :  qinpeixi                                   */
/*  Subsystem Name         :                                             */
/*  Module Name            :                                             */
/*  Language               :                                             */
/*  Target Environment     :                                             */
/*  Created Time           :  Sat 09 Mar 2013 02:23:05 PM CST            */
/*  Description            :                                             */
/*************************************************************************/

#ifndef KSERVER_SOCKET_H
#define KSERVER_SOCKET_H

#include "common/constants.h"
#include "dialog.h"
#include <QtGui/qtextedit.h>
#include <Qt/qstring.h>

class KServerSocket
{
public:
    KServerSocket();
    KServerSocket(const KServerSocket &s);
    KServerSocket operator=(const KServerSocket &s);

    // start listening
    int InitializeService(
            const char (&ccArrAddr)[g_ciAddrStrLen], 
            const int ciPort, void *dialog);
    // stop listening
    void ShutdownService() const;
    // accept one client
    int StartService(int &riAcceptSockfd) const;
    // disconnect from client
    void StopService(const int ciSockfd) const;

    void SendMsg(const int ciSockfd, 
            const char (&ccArrMsg)[g_ciMaxMsgLen]) const;
    int RecvMsg(const int ciSockfd,
            char (&rcArrMsg)[g_ciMaxMsgLen]) const;
private:
    int m_iSockfd;   //
};

#endif
