/*************************************************************************/
/* Copyright (C) Network Accessing Control -USTC, 2013                   */
/*                                                                       */
/*  File Name              :  client/KFilter.h                           */
/*  Pricipal Author        :  qinpeixi                                   */
/*  Subsystem Name         :                                             */
/*  Module Name            :                                             */
/*  Language               :                                             */
/*  Target Environment     :                                             */
/*  Created Time           :  Mon 11 Mar 2013 12:42:13 PM CST            */
/*  Description            :                                             */
/*************************************************************************/

#ifndef KFILTER_H
#define KFILTER_H

#include <netinet/in.h>
//#include <linux/netfilter.h>            /* for NF_ACCEPT */
#include <libnetfilter_queue/libnetfilter_queue.h>
#include "common/KWhiteList.h"
#include "KClient.h"
#include <Qt/qstring.h>
#include "widget.h"

class KFilter
{
public:
    KFilter();
    ~KFilter();
    KFilter(const KFilter &f);
    KFilter& operator= (const KFilter &f);

    void Initialize(Widget *widget);
    void Filter();
private:
    struct nfq_handle *m_h;
    struct nfq_q_handle *m_qh;
    struct nfnl_handle *m_nh;
    int m_fd;
    int m_rv;
    char m_buf[4096] __attribute__ ((aligned));
};

#endif
