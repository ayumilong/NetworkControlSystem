/*************************************************************************/
/* Copyright (C) Network Accessing Control -USTC, 2013                   */
/*                                                                       */
/*  File Name              :  client/client.cpp                          */
/*  Pricipal Author        :  qinpeixi                                   */
/*  Subsystem Name         :                                             */
/*  Module Name            :                                             */
/*  Language               :                                             */
/*  Target Environment     :                                             */
/*  Created Time           :  Fri 08 Mar 2013 08:47:07 PM CST            */
/*  Description            :                                             */
/*************************************************************************/

#include <pthread.h>
#include <time.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <Qt/qstring.h>
#include <QtGui/qtextedit.h>
#include "client/KClient.h"

static void* HandlePacket(void *pfilter)
{
    ((KFilter*)pfilter)->Filter();
    return NULL;
}

KClient::KClient()
{
    //this->InitWhitelist(); //Initialize whitelist, that is, add Server's MAC to whitelist
    this->enMode=CBC;
    this->pid=-1;
}

void KClient::startProtection(Widget *widget)
{
    this->kfilter=(KFilter *)new KFilter(); //Start protection
    this->kfilter->Initialize(widget);
    pthread_create(&(this->pid), NULL, &HandlePacket, (this->kfilter));
    //cout<<"In start protection, this->pid="<<this->pid<<endl;
}

void KClient::stopProtection()
{
    //cout<<"In stop protection, this->pid="<<this->pid<<endl;
    if(this->pid!=-1) //Close the filter model thread
    {
        cout<<"Stop protection! Cancle filter thread!"<<endl;
        pthread_cancel(this->pid);
        this->pid=-1;
    }
    //cout<<"Before kill filter model"<<endl;
    this->kfilter->~KFilter();
}

void KClient::InitWhitelist() //Server's MAC
{
    //unsigned char mac[g_ciMaxMacAddrLen] = {0x00, 0x1c, 0x25, 0x7c, 0x8e, 0x17};
    //unsigned char mac[g_ciMaxMacAddrLen] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char mac[g_ciMaxMacAddrLen] = {0x00, 0x1e, 0x65, 0x5a, 0x68, 0xe8};
    KMac kmac;
    kmac.SetAddr(mac);
    this->g_kwlWhitelist.Add(kmac);
}

void KClient::rsaAndDes(RSA rsa)
{
    unsigned long long tempn,tempe,temp;

    this->kcsSock.RecvMsg(this->sMsgBuf); //Recieve RSA n key
    //cout<<"After recieve RSA"<<endl;
    this->kpProto.ReadBuf(this->sMsgBuf);
    this->enMode=this->kpProto.enMode; //Set encrypt mode
    if(this->kpProto.m_cmd==RSAKEY)
    {
        cout<<"keybufn="<<this->kpProto.keybufn<<endl;
        cout<<"keybufe="<<this->kpProto.keybufe<<endl;
    }
    int i;
    tempn=0;
    for(i=0;i<strlen(this->kpProto.keybufn);i++)
    {
            tempn=tempn*10+(this->kpProto.keybufn[i]-'0');
    }
    tempe=0;
    for(i=0;i<strlen(this->kpProto.keybufe);i++)
    {
            tempe=tempe*10+(this->kpProto.keybufe[i]-'0');
    }
    rsa.setN(tempn);
    rsa.setE(tempe);

    time_t t=time(NULL);
    char *key=ctime(&t);
    strncpy(this->desKey, key, strlen(key));
    this->desKey[strlen(key)]='\0';
    int k=0;
    for(i=0;i<strlen(this->desKey);i++)
    {
        temp=rsa.encry(this->desKey[i]);
        char result2[24]="\0";
        sprintf(result2,"%lld",temp);
        for(int j=0;j<strlen(result2);j++)
        {
                this->kpProto.encryptDesKey[k++]=result2[j];
        }
        this->kpProto.encryptDesKey[k++]='a';
    }
    this->kpProto.encryptDesKey[k-1]='\0';
    this->kpProto.m_cmd=DESKEY;
    this->kpProto.WriteBuf(this->sMsgBuf);
    this->kcsSock.SendMsg(this->sMsgBuf);
    cout<<"des key="<<this->desKey<<endl;
    cout<<"rsa encrpt des key="<<this->kpProto.encryptDesKey<<endl;

}

void KClient::startClient(void *widget)
{
    unsigned char mac[g_ciMaxMacAddrLen];
    this->getLocalMac(mac);
    //printf("My mac: %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], mac[6], mac[7]);
    char ipAddress[g_ciAddrStrLen]=" ";
    strncpy(ipAddress, ((Widget *)widget)->getIpAddress(), g_ciAddrStrLen);
    if (-1 == this->kcsSock.OpenRemoteService(ipAddress, ((Widget *)widget)->getPortNumber()))
    {
        ((Widget *)widget)->setFlag(REFUSED);  //Connection refused
        return;
    }

    //cout<<"Open remote service successful!"<<endl;

    RSA rsa;
    this->rsaAndDes(rsa); //Distribute rsa key and des key

    //strncpy(this->kpProto.m_cArrUsrName, ((Widget *)widget)->getUsername(), g_ciMaxUserNameLen);
    //strncpy(this->kpProto.m_cArrPwd, ((Widget *)widget)->getPassword(), g_ciMaxPwdLen);
    if(this->enMode==CBC)
    {
        this->des.RunDes(0, 0, (unsigned char *)((Widget *)widget)->getUsername(), (unsigned char *)this->kpProto.m_cArrUsrName, strlen(((Widget *)widget)->getUsername()), (unsigned char *)desKey, strlen(desKey));
        this->des.RunDes(0, 0, (unsigned char *)((Widget *)widget)->getUsername(), (unsigned char *)this->kpProto.m_cArrPwd, strlen(((Widget *)widget)->getPassword()), (unsigned char *)desKey, strlen(desKey));
    }else
    {
        this->des.RunDes(0, 1, (unsigned char *)((Widget *)widget)->getUsername(), (unsigned char *)this->kpProto.m_cArrUsrName, strlen(((Widget *)widget)->getUsername()), (unsigned char *)desKey, strlen(desKey));
        this->des.RunDes(0, 1, (unsigned char *)((Widget *)widget)->getUsername(), (unsigned char *)this->kpProto.m_cArrPwd, strlen(((Widget *)widget)->getPassword()), (unsigned char *)desKey, strlen(desKey));
    }
    cout<<"Encrypt username="<<this->kpProto.m_cArrUsrName<<endl;
    cout<<"Encrypt password="<<this->kpProto.m_cArrPwd<<endl;
    this->kpProto.m_cmd = REG;
    this->kpProto.m_kmMac.SetAddr(mac);
    this->kpProto.WriteBuf(this->sMsgBuf);
    this->kcsSock.SendMsg(this->sMsgBuf);
    //cout<<"After send REG"<<endl;

    this->kcsSock.RecvMsg(this->sMsgBuf);
    this->kpProto.ReadBuf(this->sMsgBuf);

    if(this->kpProto.m_cmd==BADPWD)
    {
        ((Widget *)widget)->setFlag(FAIL);
        cout<<"Fail in login! CMD=BADPWD"<<endl;
    }else if(this->kpProto.m_cmd==HASLOG)
    {
        ((Widget *)widget)->setFlag(HASLOGIN);
        cout<<"User has login! CMD=HASLOGIN"<<endl;
    }else if(this->kpProto.m_cmd==REG_R)
    {
        ((Widget *)widget)->setFlag(SUCCESS);
        this->whitelistIssuePolicy=this->kpProto.whitelistIssuePolicy; //Set whitelist issue policy
        cout<<"Successful in login! CMD=SUCCESS"<<endl;
    }else
    {
        ((Widget *)widget)->setFlag(OTHER);
        cout<<"OTHER message! CMD="<<this->kpProto.m_cmd<<endl;
    }
}

void KClient::keepClient(void *widget)
{
    this->kpProto.ReadWhiteList(this->g_kwlWhitelist, this->sMsgBuf); //Read whitelist
    this->g_kwlWhitelist.PrintAll();
    cout<<"whitelist size:"<<this->g_kwlWhitelist.Count()<<endl;

    while (0 < this->kcsSock.RecvMsg(this->sMsgBuf))
    {
        this->kpProto.ReadBuf(this->sMsgBuf);
        switch(this->kpProto.m_cmd)
        {
        case ADD:
            {
                if(this->g_kwlWhitelist.Add(kpProto.m_kmMac))
                {
                    ((Widget *)widget)->getListeningFile()->append("Add a new MAC to whitelist!");
                }
                cout<<"ADD"<<endl;
                this->g_kwlWhitelist.PrintAll();
                break;
                }
        case DEL:
            {
                if(this->g_kwlWhitelist.Delete(kpProto.m_kmMac))
                {
                    ((Widget *)widget)->getListeningFile()->append("Delete a new MAC from whitelist!");
                }
                cout<<"DEL"<<endl;
                this->g_kwlWhitelist.PrintAll();
                break;
                }
        default:
            {
                cout<<"In listening: Got other message! CMD="<<this->kpProto.m_cmd<<endl;
                break;
            }
        }
    }
}


void KClient::stopClient(void *widget)
{
    enum logFlag flag=((Widget *)widget)->getFlag();
    if(flag==SUCCESS) //If a user has login
    {
        unsigned char mac[g_ciMaxMacAddrLen];
        this->getLocalMac(mac);
        //printf("My mac: %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], mac[6], mac[7]);
        this->kpProto.m_cmd = EXT;
        //strncpy(this->kpProto.m_cArrUsrName, ((Widget *)widget)->getUsername(), g_ciMaxUserNameLen);
        //strncpy(this->kpProto.m_cArrPwd, ((Widget *)widget)->getPassword(), g_ciMaxPwdLen);
        if(this->enMode==CBC)
        {
            this->des.RunDes(0, 0, (unsigned char *)((Widget *)widget)->getUsername(), (unsigned char *)this->kpProto.m_cArrUsrName, strlen(((Widget *)widget)->getUsername()), (unsigned char *)desKey, strlen(desKey));
            this->des.RunDes(0, 0, (unsigned char *)((Widget *)widget)->getUsername(), (unsigned char *)this->kpProto.m_cArrPwd, strlen(((Widget *)widget)->getPassword()), (unsigned char *)desKey, strlen(desKey));
        }else
        {
            this->des.RunDes(0, 1, (unsigned char *)((Widget *)widget)->getUsername(), (unsigned char *)this->kpProto.m_cArrUsrName, strlen(((Widget *)widget)->getUsername()), (unsigned char *)desKey, strlen(desKey));
            this->des.RunDes(0, 1, (unsigned char *)((Widget *)widget)->getUsername(), (unsigned char *)this->kpProto.m_cArrPwd, strlen(((Widget *)widget)->getPassword()), (unsigned char *)desKey, strlen(desKey));
        }
        cout<<"Encrypt username="<<this->kpProto.m_cArrUsrName<<endl;
        cout<<"Encrypt password="<<this->kpProto.m_cArrPwd<<endl;
        this->kpProto.m_kmMac.SetAddr(mac);
        this->kpProto.WriteBuf(this->sMsgBuf);
        this->kcsSock.SendMsg(this->sMsgBuf);
    }
    this->g_kwlWhitelist.deleteAll();
    this->InitWhitelist(); //Initialize whitelist, that is, add Server's MAC to whitelist

    if(!(flag==REFUSED || flag==LOGOUT) ) //If the client has connected to the remote server, we should close it
    {
	cout<<"Close remote service"<<endl;
	this->kcsSock.CloseRemoteService();
    }
}


KFilter *KClient::getKfilter()
{
    return this->kfilter;
}

void KClient::getLocalMac(unsigned char *mac_addr)
{
    int sock_mac;
    struct ifreq ifr_mac;

    sock_mac=socket(AF_INET, SOCK_STREAM, 0);
    if(sock_mac==-1)
    {
        perror("Create socket false---MAC");
    }

    memset(&ifr_mac, 0, sizeof(ifr_mac));
    strncpy(ifr_mac.ifr_name, "eth0", sizeof(ifr_mac.ifr_name)-1);
    if((ioctl(sock_mac, SIOCGIFHWADDR, &ifr_mac)) < 0)
    {
        perror("Mac ioctl error");
    }

    sprintf((char *)mac_addr,"%c%c%c%c%c%c%c%c",

      (unsigned char)ifr_mac.ifr_hwaddr.sa_data[0],

      (unsigned char)ifr_mac.ifr_hwaddr.sa_data[1],

      (unsigned char)ifr_mac.ifr_hwaddr.sa_data[2],

      (unsigned char)ifr_mac.ifr_hwaddr.sa_data[3],

      (unsigned char)ifr_mac.ifr_hwaddr.sa_data[4],

      (unsigned char)ifr_mac.ifr_hwaddr.sa_data[5], '\0', '\0');

    close( sock_mac );
}

KWhiteList KClient::getWhitelist()
{
    return this->g_kwlWhitelist;
}

void KClient::addServerMac(KMac sMac)
{
    this->g_kwlWhitelist.Add(sMac);
}
