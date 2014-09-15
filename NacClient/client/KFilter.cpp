/*************************************************************************/
/* Copyright (C) Network Accessing Control -USTC, 2013                   */
/*                                                                       */
/*  File Name              :  client/KFilter.cpp                         */
/*  Pricipal Author        :  qinpeixi                                   */
/*  Subsystem Name         :                                             */
/*  Module Name            :                                             */
/*  Language               :                                             */
/*  Target Environment     :                                             */
/*  Created Time           :  Mon 11 Mar 2013 12:46:58 PM CST            */
/*  Description            :                                             */
/*************************************************************************/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <linux/types.h>
#include <linux/netfilter.h>            /* for NF_ACCEPT */
#include <libnetfilter_queue/libnetfilter_queue.h>
#include "KFilter.h"
#include "KClient.h"
using namespace std;

extern KClient newClient;

unsigned int packetCount=0;

static int cb(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg,
        struct nfq_data *nfa, void *data)
{
    int id = 0;
    Widget *widget=(Widget *)data;
    struct nfqnl_msg_packet_hdr *ph;
    struct nfqnl_msg_packet_hw *hwph;
    KMac kmac;
    hwph = nfq_get_packet_hw(nfa);
    kmac.SetAddr(hwph->hw_addr);

    ph = nfq_get_msg_packet_hdr(nfa);
    if (ph)
        id = ntohl(ph->packet_id);
    ++packetCount;
    cout<<"In CB, packetCount="<<packetCount<<endl;
    newClient.getWhitelist().PrintAll();

    if (newClient.getWhitelist().Search(kmac))
    {
        printf("accept:");
        kmac.Print();
        
        QString acceptMac;
        acceptMac.sprintf("Packet Number: %d", packetCount);
        widget->appendLisFile(acceptMac);
	acceptMac.clear();
	acceptMac.sprintf("<font color='blue'>Accept:</font> %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x", kmac.GetAddr()[0], kmac.GetAddr()[1], kmac.GetAddr()[2], kmac.GetAddr()[3], kmac.GetAddr()[4], kmac.GetAddr()[5], kmac.GetAddr()[6], kmac.GetAddr()[7]);
	widget->appendLisFile(acceptMac);   
      
        return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
    }else //Recieve a new MAC and if the whitelistIssuePolicy==1, the client should request to the server
    {
        printf("drop:");
        kmac.Print();
      
        QString dropMac;
        dropMac.sprintf("Packet Number: %d", packetCount);
        widget->appendLisFile(dropMac);
	dropMac.clear();
	dropMac.sprintf("<font color='red'>Drop:</font> %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x", kmac.GetAddr()[0], kmac.GetAddr()[1], kmac.GetAddr()[2], kmac.GetAddr()[3], kmac.GetAddr()[4], kmac.GetAddr()[5], kmac.GetAddr()[6], kmac.GetAddr()[7]);
	widget->appendLisFile(dropMac);        

        return nfq_set_verdict(qh, id, NF_DROP, 0, NULL);
    }
}

KFilter::KFilter()
{

}

KFilter::~KFilter()
{
    nfq_destroy_queue(m_qh);
    //printf("unbinding from queue 0\n");

    system("iptables-restore < original.save");

#ifdef INSANE
    /* normally, applications SHOULD NOT issue this command, since
     * it detaches other programs/sockets from AF_INET, too ! */
    //printf("unbinding from AF_INET\n");
    nfq_unbind_pf(m_h, AF_INET);
#endif

    //printf("closing library handle\n");
    nfq_close(m_h);
    cout<<"Stop protection success!"<<endl;
}

void KFilter::Initialize(Widget *widget)
{
    //system("iptables-save > original.save");
    //printf("opening library handle\n");
    m_h = nfq_open();
    if (!m_h) {
        fprintf(stderr, "error during nfq_open()\n");
        exit(1);
    }

    //printf("unbinding existing nf_queue handler for AF_INET (if any)\n");
    if (nfq_unbind_pf(m_h, AF_INET) < 0) {
        fprintf(stderr, "error during nfq_unbind_pf()\n");
        exit(1);
    }

    //printf("binding nfnetlink_queue as nf_queue handler for AF_INET\n");
    if (nfq_bind_pf(m_h, AF_INET) < 0) {
        fprintf(stderr, "error during nfq_bind_pf()\n");
        exit(1);
    }

    //printf("binding this socket to queue '0'\n");
    m_qh = nfq_create_queue(m_h,  0, &cb, widget);
    if (!m_qh) {
        fprintf(stderr, "error during nfq_create_queue()\n");
        exit(1);
    }

    //printf("setting copy_packet mode\n");
    if (nfq_set_mode(m_qh, NFQNL_COPY_PACKET, 0xffff) < 0) {
        fprintf(stderr, "can't set packet_copy mode\n");
        exit(1);
    }

    m_fd = nfq_fd(m_h);
}

void KFilter::Filter()
{
    if (0 != system("iptables-restore < ipt.save"))
    {
        printf("Execute iptables failed.\n");
        return;
    }
    //printf("Execute iptables successfully!\n");	
   cout<<"Start protection success!"<<endl;
    while ((m_rv = recv(m_fd, m_buf, sizeof(m_buf), 0)) && m_rv >= 0) {
        nfq_handle_packet(m_h, m_buf, m_rv);
    }
}
