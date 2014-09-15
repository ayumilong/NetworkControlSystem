/*************************************************************************/
/* Copyright (C) Network Accessing Control -USTC, 2013                   */
/*                                                                       */
/*  File Name              :  server/KUsrInfo.cpp                        */
/*  Pricipal Author        :  qinpeixi                                   */
/*  Subsystem Name         :                                             */
/*  Module Name            :                                             */
/*  Language               :                                             */
/*  Target Environment     :  Linux                                      */
/*  Created Time           :  Sun 10 Mar 2013 02:19:49 PM CST            */
/*  Description            :                                             */
/*************************************************************************/

#include <iostream>
#include <fstream>
#include <tcutil.h>
#include <tchdb.h>
#include <string.h>
#include <assert.h>
#include "KUsrInfo.h"
using namespace std;

KUsrInfo::KUsrInfo()
{
    m_db = NULL;
}

KUsrInfo::~KUsrInfo()
{
    int ecode;
    TCHDB *hdb = (TCHDB*)m_db;

    if (!tchdbclose(hdb))
    {
        ecode = tchdbecode(hdb);
        cout << "Close user info database failed:"
            << tchdberrmsg(ecode) << endl;
    }
}

bool KUsrInfo::Initialize()
{
    TCHDB *hdb;
    int ecode;

    hdb = tchdbnew();
    if (!tchdbopen(hdb, g_ccArrDBName, HDBOWRITER | HDBOCREAT))
    {
        ecode = tchdbecode(hdb);
        cout << "Open user info database failed:" 
            << tchdberrmsg(ecode) << endl;
        return false;
    }

    m_db = hdb;
    ReadUsrInfoFromFile();
    return true;
}

bool KUsrInfo::AddUsr(const char (&ccArrUsrName)[g_ciMaxUserNameLen],
        const struct userInfo newUserInfo)
{
    int ecode;
    TCHDB *hdb = (TCHDB*)m_db;

    if (!tchdbput(hdb, ccArrUsrName, strlen(ccArrUsrName) + 1,
                &newUserInfo, sizeof(newUserInfo)))
    {
        ecode = tchdbecode(hdb);
        cout << "Add user failed." << tchdberrmsg(ecode) << endl;
        return false;
    }

    return true;
}

int KUsrInfo::CheckUsr(const char (&ccArrUsrName)[g_ciMaxUserNameLen],
        const char (&ccArrPwd)[g_ciMaxPwdLen], int flag)
{
    TCHDB *hdb = (TCHDB*)m_db;
    struct userInfo *newUserInfo;
    int ksize = -1;
    newUserInfo = (struct userInfo*)tchdbget(hdb, ccArrUsrName, strlen(ccArrUsrName) + 1, &ksize);

    if(newUserInfo==NULL ) //User not found, so return 1
    {
        return 1;
    }else if(newUserInfo->flag==1 && flag==1) //User has login, so return 2
    {
        return 2;
    }
    if(flag==1) //User wants to login
    {
        char *pcArrPwd = NULL;
        pcArrPwd=newUserInfo->ccArrPwd;
        if (pcArrPwd != NULL && 0 == strcmp(ccArrPwd, pcArrPwd))
        {
            newUserInfo->flag=1; //Indicate the user login
            ++(newUserInfo->totalLoginCount); //Increment the login count
            time_t t;
            t=time(NULL);
            char *tt=ctime(&t);
            strncpy(newUserInfo->lastLoginTime, tt, strlen(tt)); //Record this login time
            newUserInfo->lastLoginTime[strlen(tt)-1]='\0';
            if (!tchdbput(hdb, ccArrUsrName, strlen(ccArrUsrName) + 1,
                        newUserInfo, sizeof(struct userInfo))) //Update user information
            {
                int ecode;
                ecode = tchdbecode(hdb);
                cout << "Login and Update user failed:" << tchdberrmsg(ecode) << endl;
            }
        }else //Username/password not match
        {
            return 1;
        }

    }else //User wants to logout
    {
        newUserInfo->flag=0; //Indicate the user logout
        time_t t;
        t=time(NULL);
        char *tt=ctime(&t);
        strncpy(newUserInfo->lastLogoutTime, tt, strlen(tt)); //Record this logout time
        newUserInfo->lastLogoutTime[strlen(tt)-1]='\0';
        if (!tchdbput(hdb, ccArrUsrName, strlen(ccArrUsrName) + 1,
                    newUserInfo, sizeof(struct userInfo))) //Update user information
        {
            int ecode;
            ecode = tchdbecode(hdb);
            cout << "Logout and Update user failed:" << tchdberrmsg(ecode) << endl;
        }
    }
   return 0;
}

bool KUsrInfo::DelUsr(const char (&ccArrUsrName)[g_ciMaxUserNameLen])
{
    int ecode;
    TCHDB *hdb = (TCHDB*)m_db;

    if (!tchdbout2(hdb, ccArrUsrName))
    {
        ecode = tchdbecode(hdb);
        cout << "Delete user failed:" << tchdberrmsg(ecode) << endl;
        return false;
    }

    return true;
}

void KUsrInfo::ReadUsrInfoFromFile()
{
    ifstream file(g_ccArrUsrInfoFile);
    char cArrUsrName[g_ciMaxUserNameLen];
    struct userInfo newUserInfo;
    if(!file.is_open())
    {
        cout<<"Fail in reading user database!"<<endl;
        return;
    }
    while (!file.eof())
    {
        file >> cArrUsrName;
        file >> newUserInfo.ccArrPwd;
        file >> newUserInfo.flag;
        file >> newUserInfo.lastLoginTime;
        file >> newUserInfo.lastLogoutTime;
        file >> newUserInfo.totalLoginCount;
        file >> newUserInfo.totalOnlineTime;
        AddUsr(cArrUsrName, newUserInfo);
    }
}

void* KUsrInfo::getDb()
{
    return this->m_db;
}
