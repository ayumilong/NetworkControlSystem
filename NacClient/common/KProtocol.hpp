/*************************************************************************/
/* Copyright (C) Network Accessing Control -USTC, 2013                   */
/*                                                                       */
/*  File Name              :  common/KProtocol.h                         */
/*  Pricipal Author        :  qinpeixi                                   */
/*  Subsystem Name         :                                             */
/*  Module Name            :                                             */
/*  Language               :                                             */
/*  Target Environment     :                                             */
/*  Created Time           :  Sat 09 Mar 2013 09:57:06 PM CST            */
/*  Description            :                                             */
/*************************************************************************/

#ifndef KPROTOCOL_H
#define KPROTOCOL_H

#include <string.h>
#include <stdio.h>
#include "constants.h"
#include "KMac.hpp"
#include "KWhiteList.h"
#include "./widget.h"

enum ECmd {
    REG=1, REG_R, // register request from client to server
    BADPWD, HASLOG,    // no username/password pair message from server to client
    ADD, ADD_R, // message of adding or deleting a mac from server to client
    DEL, DEL_R,
    EXT,
    RSAKEY, DESKEY
};

class KProtocol
{
public:
    KProtocol() {}
    KProtocol(const KProtocol &p);
    KProtocol operator=(const KProtocol &p);

    void WriteBuf(char (&rcArrBuf)[g_ciMaxMsgLen]) {
        //m_iSize = sizeof(*this);
        memcpy(rcArrBuf, this, sizeof(*this));
    }
    void ReadBuf(const char (&ccArrBuf)[g_ciMaxMsgLen]) {
        memcpy(this, ccArrBuf, sizeof(*this));
    }
    bool WriteWhiteList(char (&rcArrBuf)[g_ciMaxMsgLen], 
            const KWhiteList &cWhitelist) {
        int offset = 3*sizeof(int) + sizeof(ECmd);
        if (cWhitelist.Count() * sizeof(KMac) > (g_ciMaxMsgLen - offset)) 
            return false;
        m_iMacCnt = cWhitelist.Count();
        WriteBuf(rcArrBuf);
        cWhitelist.Serialize(rcArrBuf + offset);

        return true;
    }
    bool ReadWhiteList(KWhiteList &rWhitelist,
            const char (&ccArrBuf)[g_ciMaxMsgLen]) {
        int offset = 3*sizeof(int) + sizeof(ECmd);
        ReadBuf(ccArrBuf);
        rWhitelist.Deserialize(ccArrBuf + offset, m_iMacCnt);
        return true;
    }

    enum ECmd m_cmd;
    enum wlIssuePolicy whitelistIssuePolicy;
    enum encryptMode enMode;
    int m_iMacCnt; // used for tranmitting whitelist
    KMac m_kmMac;
    char m_cArrUsrName[g_ciMaxUserNameLen];
    char m_cArrPwd[g_ciMaxPwdLen];
    char keybufn[RSA_NKEY_LEN];
    char keybufe[RSA_EKEY_LEN];
    char encryptDesKey[TEMP_KEY_LEN];
};


#endif
