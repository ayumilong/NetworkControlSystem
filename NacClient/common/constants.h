/*************************************************************************/
/* Copyright (C) Network Accessing Control -USTC, 2013                   */
/*                                                                       */
/*  File Name              :  common/constants.h                         */
/*  Pricipal Author        :  qinpeixi                                   */
/*  Subsystem Name         :                                             */
/*  Module Name            :                                             */
/*  Language               :                                             */
/*  Target Environment     :                                             */
/*  Created Time           :  Sat 09 Mar 2013 02:34:29 PM CST            */
/*  Description            :                                             */
/*************************************************************************/

#ifndef CONSTANTS_H
#define CONSTANTS_H

const int g_ciAddrStrLen                        = 20;
const int g_ciMaxMsgLen                         = 1024;
const int g_ciMaxQueueLen                       = 5;
const int g_ciMaxMacAddrLen                     = 8;
const int g_ciMaxUserNameLen                    = 128;
const int g_ciMaxPwdLen                         = 128;

const int RSA_NKEY_LEN                          =  40;
const int RSA_EKEY_LEN                          =  40;
const int DES_KEY_LEN                           =  48;
const int TEMP_KEY_LEN                          = 400;

const char g_ccArrLocalAddr[g_ciAddrStrLen]     = "10.211.55.4";
const int g_ciPort                              = 5000;

const char g_ccArrDBName[]                      = "NACUsrInfo.db";
const char g_ccArrUsrInfoFile[]                 = "userinfo";

#endif
