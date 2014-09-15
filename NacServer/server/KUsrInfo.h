/*************************************************************************/
/* Copyright (C) Network Accessing Control -USTC, 2013                   */
/*                                                                       */
/*  File Name              :  server/KUsrInfo.h                          */
/*  Pricipal Author        :  qinpeixi                                   */
/*  Subsystem Name         :                                             */
/*  Module Name            :                                             */
/*  Language               :                                             */
/*  Target Environment     :                                             */
/*  Created Time           :  Sun 10 Mar 2013 01:59:53 PM CST            */
/*  Description            :                                             */
/*************************************************************************/

#ifndef KUSRINFO_H
#define KUSRINFO_H

#include "common/constants.h"

typedef void* DataBase;

struct userInfo
{
    char ccArrPwd[g_ciMaxPwdLen];
    char lastLoginTime[g_ciMaxTimeLen];
    char lastLogoutTime[g_ciMaxTimeLen];
    unsigned int totalOnlineTime;
    unsigned int totalLoginCount;
    int flag; //Indicate if the user is login or not
};

class KUsrInfo
{
public:
    KUsrInfo();
    ~KUsrInfo();
    KUsrInfo(const KUsrInfo &u);
    KUsrInfo operator=(const KUsrInfo &u);

    bool Initialize();
    bool AddUsr(const char (&ccArrUsrName)[g_ciMaxUserNameLen],
            const struct userInfo newUserIfo
          );
    int CheckUsr(const char (&ccArrUsrName)[g_ciMaxUserNameLen],
            const char (&ccArrPwd)[g_ciMaxPwdLen], int flag
          );
    bool DelUsr(const char (&ccArrUsrName)[g_ciMaxUserNameLen]);

    void* getDb();

private:
    DataBase m_db;

    void ReadUsrInfoFromFile();
};

#endif
