#include "server/KServer.h"
#include "dialog.h"
#include "common/KWhiteList.h"

#include <time.h>
#include <pthread.h>

int onlineUserCount=0;

extern KServer newServer;

KServer::KServer()
{
    this->InitWhitelist();
    this->serverLog.InitLogCfg(LEVEL_DEBUG, "log/serverLogFile", 5*1024*1024);
    onlineUserCount=0;
}


void KServer::notifyAllClients(ECmd cmd, struct clientInfo* client)
{
    client->kpProto.m_cmd = cmd;
    bzero(client->kpProto.m_cArrUsrName, g_ciMaxUserNameLen);
    bzero(client->kpProto.m_cArrPwd, g_ciMaxPwdLen);
    client->kpProto.WriteBuf(client->sMsgBuf);
    Dialog *dialog=client->dialog;
    int length;
    if(cmd==DEL)
    {
        length=client->vClientSockfds->size(); //Notify all the other clients to delete a MAC
        this->serverLog.WriteLog(LEVEL_INFO, "Notify all the other clients to delete a MAC: %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x", (client->kpProto.m_kmMac.GetAddr())[0], (client->kpProto.m_kmMac.GetAddr())[1],(client->kpProto.m_kmMac.GetAddr())[2],(client->kpProto.m_kmMac.GetAddr())[3],(client->kpProto.m_kmMac.GetAddr())[4],(client->kpProto.m_kmMac.GetAddr())[5], (client->kpProto.m_kmMac.GetAddr())[6], (client->kpProto.m_kmMac.GetAddr())[7]);
    }else
    {
        if(dialog->getWlIssuePolicy()==BROADCAST) //The server will broadcast a new MAC to all the clients
        {
            length=client->vClientSockfds->size();
        }else
        {
            length=client->vClientSockfds->size()>1 ? 1 :client->vClientSockfds->size();
        }
        this->serverLog.WriteLog(LEVEL_INFO, "Notify all the other clients to add a new MAC: %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x", (client->kpProto.m_kmMac.GetAddr())[0], (client->kpProto.m_kmMac.GetAddr())[1],(client->kpProto.m_kmMac.GetAddr())[2],(client->kpProto.m_kmMac.GetAddr())[3],(client->kpProto.m_kmMac.GetAddr())[4],(client->kpProto.m_kmMac.GetAddr())[5], (client->kpProto.m_kmMac.GetAddr())[6], (client->kpProto.m_kmMac.GetAddr())[7]);
    }
    for (size_t i=0; i<length; ++i)
    {
        kssSocket.SendMsg(client->vClientSockfds->at(i), client->sMsgBuf);
    }    
}


void* connectClient(void *newClient)
{
    struct clientInfo* client=(struct clientInfo*)newClient;
    int client_sock=client->client_sock;
    KWhiteList *g_kwlWhitelist=client->g_kwlWhitelist;
    KServerSocket *kssSocket=client->kssSocket;
    vector<int> *vClientSockfds=client->vClientSockfds;
    Dialog *dialog=client->dialog;

   
    long long temp; //Distribute the rsa key and recieve des key
    CDES des;
    char desKey[DES_KEY_LEN],*pch;

    sprintf(client->kpProto.keybufn,"%lld",client->rsaKeyN);
    cout<<client->kpProto.keybufn<<endl;
    sprintf(client->kpProto.keybufe,"%lld",client->rsaKeyE);
    cout<<client->kpProto.keybufe<<endl;
    client->kpProto.m_cmd=RSAKEY;
    client->kpProto.enMode=dialog->getEnMode(); //Set encrypt mode
    client->kpProto.WriteBuf(client->sMsgBuf);
    kssSocket->SendMsg(client_sock, client->sMsgBuf);
    cout<<"After send RSA"<<endl;

    kssSocket->RecvMsg(client_sock, client->sMsgBuf);
    client->kpProto.ReadBuf(client->sMsgBuf);
    if(client->kpProto.m_cmd==DESKEY)
    {
        QString s;
        s.sprintf("%sRecieved a RSA encrypt DES key: %s", "\n", client->kpProto.encryptDesKey);
        dialog->appendLisFile(s);
        client->serverLog->WriteLog(LEVEL_INFO, s.toStdString().c_str());

        pch=strtok(client->kpProto.encryptDesKey, "a");
        int i=0;
        while(pch!=NULL)
        {
            temp=0;
            while(*pch!='\0')
            {
                    temp=temp*10+(*pch-'0');
                    pch++;
            }
            desKey[i]=(char)client->rsa->decry(temp);
            pch=strtok(NULL,"a");
            i++;
        }
        desKey[i]='\0';
        s.clear();
        s.sprintf("%sUse RSA to decrypt the DES key: %s", "\n", desKey);
        dialog->appendLisFile(s);
        client->serverLog->WriteLog(LEVEL_INFO, s.toStdString().c_str());
    }
         
    while (0 < kssSocket->RecvMsg(client_sock, client->sMsgBuf))
    {
        client->kpProto.ReadBuf(client->sMsgBuf);
	cout<<"cmd="<<client->kpProto.m_cmd<<endl;

   
        switch(client->kpProto.m_cmd)
        {
        case REG:
            {
                KUsrInfo kuUsr;
                kuUsr.Initialize(); //Open database

                QString s;
                s.sprintf("%sRecieved username= %s%spassword=%s", "\n", client->kpProto.m_cArrUsrName, "   ", client->kpProto.m_cArrPwd);
                //dialog->appendLisFile(s);

                char username[g_ciMaxUserNameLen];
                char password[g_ciMaxPwdLen];
                if(dialog->getEnMode()==CBC)
                {
                    des.RunDes(1, 0, (unsigned char *)client->kpProto.m_cArrUsrName, (unsigned char *)username, strlen(client->kpProto.m_cArrUsrName), (unsigned char *)desKey, strlen(desKey));
                    des.RunDes(1, 0, (unsigned char *)client->kpProto.m_cArrPwd, (unsigned char *)password, strlen(client->kpProto.m_cArrPwd), (unsigned char *)desKey, strlen(desKey));
                }else
                {
                    des.RunDes(1, 1, (unsigned char *)client->kpProto.m_cArrUsrName, (unsigned char *)username, strlen(client->kpProto.m_cArrUsrName), (unsigned char *)desKey, strlen(desKey));
                    des.RunDes(1, 1, (unsigned char *)client->kpProto.m_cArrPwd, (unsigned char *)password, strlen(client->kpProto.m_cArrPwd), (unsigned char *)desKey, strlen(desKey));
                }

                cout<<"username="<<username<<endl;
                cout<<"password="<<password<<endl;
                cout<<"recieve name="<<client->kpProto.m_cArrUsrName<<endl;
                cout<<"recieve password="<<client->kpProto.m_cArrPwd<<endl;
                int flag=kuUsr.CheckUsr(username, password, 1);

                if (flag==1)
                {
                    client->serverLog->WriteLog(LEVEL_ERROR, "Fail in login! Username/password does not match.");
                    dialog->appendLisFile("<font color='red'>Fail in login. Username/password does not match.</font>");
                    client->kpProto.m_cmd = BADPWD;
                    client->kpProto.WriteBuf(client->sMsgBuf);
                    kssSocket->SendMsg(client_sock, client->sMsgBuf);
                }else if(flag==2)
                {
                    QString s;
                    s.sprintf("<font color='red'>%s has login. Repeat login is prohibited!</font>", username);
                    dialog->appendLisFile(s);
                    s.clear();
                    s.sprintf("%s has login. Repeat login is prohibited!", username);
                    client->serverLog->WriteLog(LEVEL_ERROR, s.toStdString().c_str());
                    client->kpProto.m_cmd = HASLOG;
                    client->kpProto.WriteBuf(client->sMsgBuf);
                    kssSocket->SendMsg(client_sock, client->sMsgBuf);
                }else if(flag==0)
                {
                    ++(onlineUserCount);  //Increment the online user count
                    time_t t;
                    t=time(NULL);
                    QString userInfo;
                    userInfo.sprintf("<font color='red'>%s</font> login. Total online users: <font color='blue'>%d</font>-----%s", username, onlineUserCount, ctime(&t));
                    dialog->appendLisFile(userInfo);
                    userInfo.clear();
                    userInfo.sprintf("%s login. Total online users: %d.", username, onlineUserCount);
                    client->serverLog->WriteLog(LEVEL_INFO, userInfo.toStdString().c_str());

                    // notify all the other clients
                    client->dialog=dialog;
                    newServer.notifyAllClients(ADD, client);


                    vClientSockfds->push_back(client_sock);
                    g_kwlWhitelist->Add(client->kpProto.m_kmMac);

                    client->kpProto.m_cmd = REG_R;
                    client->kpProto.whitelistIssuePolicy=dialog->getWlIssuePolicy(); //Whitelist issue policy
                    client->kpProto.WriteBuf(client->sMsgBuf);
                    client->kpProto.WriteWhiteList(client->sMsgBuf, *g_kwlWhitelist);
                    kssSocket->SendMsg(client_sock, client->sMsgBuf);
                }
                break;
             }
        case EXT:
            {

                KUsrInfo kuUsr;
                kuUsr.Initialize();
                char username[g_ciMaxUserNameLen];
                char password[g_ciMaxPwdLen];

                if(dialog->getEnMode()==CBC)
                {
                    des.RunDes(1, 0, (unsigned char *)client->kpProto.m_cArrUsrName, (unsigned char *)username, strlen(client->kpProto.m_cArrUsrName), (unsigned char *)desKey, strlen(desKey));
                    des.RunDes(1, 0, (unsigned char *)client->kpProto.m_cArrPwd, (unsigned char *)password, strlen(client->kpProto.m_cArrPwd), (unsigned char *)desKey, strlen(desKey));
                }else
                {
                    des.RunDes(1, 1, (unsigned char *)client->kpProto.m_cArrUsrName, (unsigned char *)username, strlen(client->kpProto.m_cArrUsrName), (unsigned char *)desKey, strlen(desKey));
                    des.RunDes(1, 1, (unsigned char *)client->kpProto.m_cArrPwd, (unsigned char *)password, strlen(client->kpProto.m_cArrPwd), (unsigned char *)desKey, strlen(desKey));
                }

                cout<<"username="<<username<<endl;
                cout<<"password="<<password<<endl;
                cout<<"recieve name="<<client->kpProto.m_cArrUsrName<<endl;
                cout<<"recieve password="<<client->kpProto.m_cArrPwd<<endl;
                kuUsr.CheckUsr(username, password, 0); //Record user logout information

                --(onlineUserCount);
                time_t t;
                t=time(NULL);
                QString userInfo;
                userInfo.sprintf("<font color='red'>%s</font> logout. Total online users: <font color='blue'>%d</font>-----%s", username, onlineUserCount, ctime(&t));

                dialog->appendLisFile(userInfo);
                userInfo.clear();
                userInfo.sprintf("%s logout. Total online users: %d.", username, onlineUserCount);
                client->serverLog->WriteLog(LEVEL_INFO, userInfo.toStdString().c_str());

                g_kwlWhitelist->Delete(client->kpProto.m_kmMac); //Delete the MAC
                userInfo.clear();
                userInfo.sprintf("Delete the MAC from whitelist: %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x", (client->kpProto.m_kmMac.GetAddr())[0], (client->kpProto.m_kmMac.GetAddr())[1],(client->kpProto.m_kmMac.GetAddr())[2],(client->kpProto.m_kmMac.GetAddr())[3],(client->kpProto.m_kmMac.GetAddr())[4],(client->kpProto.m_kmMac.GetAddr())[5], (client->kpProto.m_kmMac.GetAddr())[6], (client->kpProto.m_kmMac.GetAddr())[7]);
                client->serverLog->WriteLog(LEVEL_INFO, userInfo.toStdString().c_str());

                vector<int>::iterator itr=vClientSockfds->begin(); //Delete the client sock id
                while(itr!=vClientSockfds->end())
                {
                    if(*itr==client_sock)
                    {
                        vClientSockfds->erase(itr);
                        break;
                    }
                    ++itr;
                }
                userInfo.clear();
                userInfo.sprintf("Delete the client sock ID from clients queue.");
                client->serverLog->WriteLog(LEVEL_INFO, userInfo.toStdString().c_str());

                client->dialog=dialog;
                newServer.notifyAllClients(DEL, client); //Notify all other clients to delete this MAC

                pthread_exit(NULL);
                break;
            }
        default:
            break;     
        }
    }
    pthread_exit(NULL);
}

void KServer::startServer(void *dialog)
{
    char ccArrLocalAddr[g_ciAddrStrLen]     = "";
    int ciPort;
    strncpy(ccArrLocalAddr, ((Dialog *)dialog)->getIpAddr(), g_ciAddrStrLen);
    ciPort=((Dialog *)dialog)->getPort();

    if (-1 == kssSocket.InitializeService(ccArrLocalAddr, ciPort, dialog))  //服务器启动监听
        return;
    this->serverLog.WriteLog(LEVEL_INFO, "Server start listening!");

    while(true)
    {
        int client_sock;
        kssSocket.StartService(client_sock); //Accept a new client
        struct clientInfo client;
        client.client_sock=client_sock;
        client.g_kwlWhitelist=&(this->g_kwlWhitelist);
        client.kssSocket=&(this->kssSocket);
        client.vClientSockfds=&(this->vClientSockfds);
        client.rsa=&(this->rsa);
        client.rsaKeyN=this->rsaKeyN;
        client.rsaKeyE=this->rsaKeyE;
        client.dialog=(Dialog *)dialog;
        client.serverLog=&(this->serverLog);

        pthread_t pt;
        pthread_attr_t attr;  //The attribute of the thread
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        pthread_create(&pt, &attr, &connectClient, &client); //Construct a new client
        pthread_attr_destroy(&attr);
    }
}

void KServer::stopServer()
{
    kssSocket.ShutdownService();
}

void KServer::InitWhitelist()
{
    //unsigned char mac[g_ciMaxMacAddrLen] = {0x00, 0x26, 0x82, 0x03, 0x20, 0xbd};
    unsigned char mac[g_ciMaxMacAddrLen] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    KMac kmac;
    kmac.SetAddr(mac);
    this->g_kwlWhitelist.Add(kmac);
}

KWhiteList KServer::getKWhitelist()
{
    return this->g_kwlWhitelist;
}

vector<int> KServer::getClientSocket()
{
    return this->vClientSockfds;
}

bool KServer::addWhitelist(const KMac &kmac)
{
    return this->g_kwlWhitelist.Add(kmac);
}

bool KServer::deleteWhitelist(const KMac &kmac)
{
    return this->g_kwlWhitelist.Delete(kmac);
}

void KServer::clearWhitelist()
{
    this->g_kwlWhitelist.deleteAll();
}

unsigned long long KServer::getRsaKeyN()
{
    return this->rsaKeyN;
}

unsigned long long KServer::getRsaKeyE()
{
    return this->rsaKeyE;
}

void KServer::setRsaKeyN(unsigned long long keyN)
{
    this->rsaKeyN=keyN;
}

void KServer::setRsaKeyE(unsigned long long keyE)
{
    this->rsaKeyE=keyE;
}

RSA KServer::getRsa()
{
    return this->rsa;
}

void KServer::generateRsaKey()
{
    this->rsa.generatekey();
}
