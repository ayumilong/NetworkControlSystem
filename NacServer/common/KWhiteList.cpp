/*************************************************************************/
/* Copyright (C) Network Accessing Control -USTC, 2013                   */
/*                                                                       */
/*  File Name              :  common/KWhiteList.cpp                      */
/*  Pricipal Author        :  qinpeixi                                   */
/*  Subsystem Name         :                                             */
/*  Module Name            :                                             */
/*  Language               :                                             */
/*  Target Environment     :                                             */
/*  Created Time           :  Sun 10 Mar 2013 08:05:13 PM CST            */
/*  Description            :                                             */
/*************************************************************************/

#include <iostream>
#include <algorithm>
#include <pthread.h>
#include "KWhiteList.h"
using namespace std;

static pthread_mutex_t g_wlMutex;

KWhiteList::KWhiteList()
{
    pthread_mutex_init(&g_wlMutex, NULL);
}

bool KWhiteList::Search(const KMac &cMac) 
{
    bool res;
    pthread_mutex_lock(&g_wlMutex);
    vector<KMac>::const_iterator pos =
        find(m_cWhitelist.begin(), m_cWhitelist.end(), cMac);
    if (pos == m_cWhitelist.end())
        res = false;
    else
        res = true;
    pthread_mutex_unlock(&g_wlMutex);
    return res;
}

bool KWhiteList::Add(const KMac &cMac)
{
    if(!Search(cMac)) //If cMac is not in the whitelist, add it
    {
        pthread_mutex_lock(&g_wlMutex);
        m_cWhitelist.push_back(cMac);
        pthread_mutex_unlock(&g_wlMutex);
        return true;
    }else
    {
        return false;
    }
}


bool KWhiteList::Delete(const KMac &cMac)
{
    bool res;
    pthread_mutex_lock(&g_wlMutex);
    vector<KMac>::iterator pos =
        find(m_cWhitelist.begin(), m_cWhitelist.end(), cMac);
    if (pos != m_cWhitelist.end())
    {
        m_cWhitelist.erase(pos);
        res = true;
    }
    else
        res = false;
    pthread_mutex_unlock(&g_wlMutex);
    return res;
}

void KWhiteList::deleteAll()
{
    this->m_cWhitelist.clear();
}

vector<KMac> KWhiteList::getWhitelist()
{
    return this->m_cWhitelist;
}
