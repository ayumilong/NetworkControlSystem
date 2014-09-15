/*************************************************************************/
/* Copyright (C) Network Accessing Control -USTC, 2013                   */
/*                                                                       */
/*  File Name              :  server/KServerSocket.cpp                    */
/*  Pricipal Author        :  qinpeixi                                   */
/*  Subsystem Name         :                                             */
/*  Module Name            :                                             */
/*  Language               :  C                                          */
/*  Target Environment     :  Linux                                      */
/*  Created Time           :  Sat 09 Mar 2013 02:22:46 PM CST            */
/*  Description            :                                             */
/*************************************************************************/

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <assert.h>
#include "KServerSocket.h"

using namespace std;

KServerSocket::KServerSocket()
{
    m_iSockfd = 0;
}

KServerSocket::KServerSocket(const KServerSocket &s)
{
    m_iSockfd = s.m_iSockfd;
}

KServerSocket KServerSocket::operator=(const KServerSocket &s)
{
    m_iSockfd = s.m_iSockfd;

    return *this;
}

int KServerSocket::InitializeService(
        const char (&ccArrAddr)[g_ciAddrStrLen], 
        const int ciPort, void *dialog)
{
    //cout<<"In InitializeService"<<endl;
    struct sockaddr_in sa;
    int opt = 1;
    socklen_t len = sizeof(opt);

    m_iSockfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(m_iSockfd, SOL_SOCKET, SO_REUSEADDR, &opt, len);
    if (m_iSockfd == -1)
    {
        perror("socket");
        return -1;
    }

    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(ciPort);
    sa.sin_addr.s_addr = strlen(ccArrAddr) > 0
        ? inet_addr(ccArrAddr) : INADDR_ANY;
    //memset(&sa.sin_zero, 0, 8);

    int res;

    res = bind(m_iSockfd, (struct sockaddr *)&sa, sizeof(sa));
    if (res != 0)
    {
        perror("bind");
        return -1;
    }

    res = listen(m_iSockfd, g_ciMaxQueueLen);
    if (res != 0)
    {
        perror("listen");
        return -1;
    }
	
    struct sockaddr_in sin;
    len = sizeof(sin);
    getsockname(m_iSockfd, (struct sockaddr *)&sin, &len);
    QString ipPort;
    ipPort.sprintf("Server address----%s:%d", inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));
    ((Dialog *)dialog)->appendLisFile(ipPort);;
    return 0;
}


void KServerSocket::ShutdownService() const
{
    close(m_iSockfd);
}

int KServerSocket::StartService(int &riAcceptSockfd) const
{
    struct sockaddr_in sa;
    socklen_t len = sizeof(struct sockaddr);
    riAcceptSockfd = accept(m_iSockfd, (struct sockaddr *)&sa, &len);
    if (riAcceptSockfd == -1)
    {
        perror("accept");
        return -1;
    }
    return 0;
}

void KServerSocket::StopService(const int ciSockfd) const
{
    close(ciSockfd);
}

void KServerSocket::SendMsg(const int ciSockfd, 
        const char (&ccArrMsg)[g_ciMaxMsgLen]) const
{
    assert(ciSockfd != -1);
    int res = send(ciSockfd, ccArrMsg, g_ciMaxMsgLen, 0);
    if (res == -1) 
        perror("send");
}

int KServerSocket::RecvMsg(const int ciSockfd,
        char (&rcArrMsg)[g_ciMaxMsgLen]) const
{
    assert(ciSockfd != -1);
    int res=0;
    res = recv(ciSockfd, rcArrMsg, g_ciMaxMsgLen, 0);
    if (res < 0)
        perror("receive");
    return res;
}

