/*************************************************************************/
/* Copyright (C) Network Accessing Control -USTC, 2013                   */
/*                                                                       */
/*  File Name              :  client/KClientSocket.cpp                   */
/*  Pricipal Author        :  qinpeixi                                   */
/*  Subsystem Name         :                                             */
/*  Module Name            :                                             */
/*  Language               :                                             */
/*  Target Environment     :                                             */
/*  Created Time           :  Sat 09 Mar 2013 08:13:43 PM CST            */
/*  Description            :                                             */
/*************************************************************************/

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <assert.h>
#include "KClientSocket.h"

using namespace std;

KClientSocket::KClientSocket()
{
    m_iSockfd = 0;
}

KClientSocket::KClientSocket(const KClientSocket &s)
{
    m_iSockfd = s.m_iSockfd;
}

KClientSocket KClientSocket::operator=(const KClientSocket &s)
{
    m_iSockfd = s.m_iSockfd;

    return *this;
}

int KClientSocket::OpenRemoteService(
        const char (&ccArrAddr)[g_ciAddrStrLen], 
        const int ciPort)
{
    struct sockaddr_in sa;

    m_iSockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_iSockfd == -1)
    {
        perror("int");
        return -1;
    }

    sa.sin_family = AF_INET;
    sa.sin_port = htons(ciPort);
    sa.sin_addr.s_addr = strlen(ccArrAddr) > 0 
        ? inet_addr(ccArrAddr) : inet_addr(g_ccArrLocalAddr);
    memset(&sa.sin_zero, 0, 8);

    int res;

    res = connect(m_iSockfd, (struct sockaddr *)&sa, sizeof(struct sockaddr));
    if (res == -1)
    {
        perror("connet");
        return -1;
    }
    return 0;
}

void KClientSocket::CloseRemoteService() const
{
    close(m_iSockfd);
}

void KClientSocket::SendMsg(const char (&ccArrMsg)[g_ciMaxMsgLen]) const
{
    assert(m_iSockfd != -1);
    int res = send(m_iSockfd, ccArrMsg, g_ciMaxMsgLen, 0);
    if (res == -1) 
        perror("send");
}

int KClientSocket::RecvMsg(char (&cArrMsg)[g_ciMaxMsgLen]) const
{
    assert(m_iSockfd != -1);
    int res = recv(m_iSockfd, cArrMsg, g_ciMaxMsgLen, 0);
    if (res < 0)
        perror("send");
    
    return res;
}


int KClientSocket::getSockfd()
{
    return this->m_iSockfd;
}
