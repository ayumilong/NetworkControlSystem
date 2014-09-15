/*************************************************************************/
/* Copyright (C) Network Accessing Control -USTC, 2013                   */
/*                                                                       */
/*  File Name              :  common/KWhiteList.h                        */
/*  Pricipal Author        :  qinpeixi                                   */
/*  Subsystem Name         :                                             */
/*  Module Name            :                                             */
/*  Language               :                                             */
/*  Target Environment     :                                             */
/*  Created Time           :  Sun 10 Mar 2013 07:58:35 PM CST            */
/*  Description            :                                             */
/*************************************************************************/

#ifndef KWHITELIST_H
#define KWHITELIST_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "constants.h"
#include "KMac.hpp"
using namespace std;

class KWhiteList
{
public:
    KWhiteList();
    //KWhiteList(const KWhiteList &wl);
    KWhiteList& operator=(const KWhiteList &wl);

    void deleteAll();
    bool Search(const KMac &cMac);
    bool Add(const KMac &cMac);
    bool Delete(const KMac &cMac); 
    int Count() const { return m_cWhitelist.size(); }
    void PrintAll() const {
        for (unsigned i=0; i<m_cWhitelist.size(); i++)
            m_cWhitelist[i].Print();
    }
    void Serialize(char const *pcBuf) const {
        copy(m_cWhitelist.begin(), m_cWhitelist.end(), (KMac*)pcBuf);
    }
    void Deserialize(const char *pccBuf, const int ciCnt) {
        copy((KMac*)pccBuf, (KMac*)(pccBuf + ciCnt * sizeof(KMac)), 
                back_inserter(m_cWhitelist));
    }
    vector<KMac> getWhitelist();

private:
    vector<KMac> m_cWhitelist;
};

#endif
