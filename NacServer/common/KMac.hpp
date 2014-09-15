/*************************************************************************/
/* Copyright (C) Network Accessing Control -USTC, 2013                   */
/*                                                                       */
/*  File Name              :  common/KMac.hpp                            */
/*  Pricipal Author        :  qinpeixi                                   */
/*  Subsystem Name         :                                             */
/*  Module Name            :                                             */
/*  Language               :                                             */
/*  Target Environment     :                                             */
/*  Created Time           :  Sun 10 Mar 2013 08:25:21 PM CST            */
/*  Description            :                                             */
/*************************************************************************/

#ifndef KMAC_HPP
#define KMAC_HPP

#include <string.h>
#include "constants.h"
#include <stdio.h>

#define ACTUAL_MAC_LEN 6

class KMac
{
public:
    KMac(){}
    KMac(const KMac &mac) {
        memcpy(m_cArrAddr, mac.m_cArrAddr, g_ciMaxMacAddrLen);
    }
    KMac &operator=(const KMac &mac) {
        memcpy(m_cArrAddr, mac.m_cArrAddr, g_ciMaxMacAddrLen);
        return *this;
    }
    bool operator==(const KMac &mac) {
        if (0 == memcmp(m_cArrAddr, mac.m_cArrAddr, ACTUAL_MAC_LEN))
            return true;
        else
            return false;
    }
    void SetAddr(const unsigned char (&ccArrAddr)[g_ciMaxMacAddrLen]) {
        memcpy(m_cArrAddr, ccArrAddr, g_ciMaxMacAddrLen);
    }
    unsigned char *GetAddr() {return m_cArrAddr;}
    void Print() const {
        for (int i=0; i<g_ciMaxMacAddrLen; i++)
            printf("%02x ", m_cArrAddr[i]);
        printf("\n");
    }
            
private:
    unsigned char m_cArrAddr[g_ciMaxMacAddrLen];
};

#endif
