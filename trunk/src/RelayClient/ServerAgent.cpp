/*
 * =====================================================================================
 *
 *       Filename:  ServerAgent.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/05/13 04:19:46
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Heaven (), zhanwenhan@163.com
 *        Company:  NDSL
 *
 * =====================================================================================
 */

#include "ServerAgent.h"

#include "Protocol/Protocol.h"
#include "common/comm/Epoll.h"
#include "common/comm/SocketAddress.h"
#include "common/log/log.h"

#include <iostream>
using namespace std;


const unsigned int buflen = 10240000;

ServerAgent::ServerAgent():
    m_num(0)
{

}

ServerAgent::ServerAgent(const SocketAddress &addr):
    TCPAgent(addr),
    m_num(0)
{

}

ServerAgent::ServerAgent(const TCPSocket &sock, const SocketAddress &addr):
    TCPAgent(sock,addr),
    m_num(0)
{

}

ServerAgent::~ServerAgent()
{
}

    int
ServerAgent::init()
{
    if ( this->connect() != SUCCESSFUL )
    {
        ERROR_LOG("in ServerAgent::init connect server error");
        return FAILED;
    }

    return SUCCESSFUL;
}

    int
ServerAgent::connectAfter( bool bConnect)
{
    if (bConnect )
    {
        m_num++;


        MsgHeader msg;
        msg.cmd = REGISTER;
        msg.length = buflen;
        msg.para1 = m_num;
        msg.para2 = getID();

        char* buf = new char[buflen + 1];
        memset(buf, 0, buflen + 1);

        memset(buf , '$', buflen);

        sendPackage(msg,buf);

        delete []buf;
        buf = NULL;

        return SUCCESSFUL;
    }
    else
    {
        return FAILED;
    }
}
    void
ServerAgent::readBack(InReq &req)
{
    if(req.m_msgHeader.cmd == REGISTERACK)
    {
        cout<<"clientId "<<getID()<<" register successful"<<endl;
    }
    else if(req.m_msgHeader.cmd == READOFFLINENEWS)
    {
        cout<<"offile message length:";
        cout<<req.m_msgHeader.length<<endl;
    }
    else
    {
//    cout<<"clientId "<<getID()<<" recv data:"<<req.ioBuf<<endl;
    }
    uint32_t toClientId = 0;
    char ch;

    if(m_ID %2 )
    {
        toClientId = m_ID + 1;
        ch = 'A';
    }
    else
    {
        toClientId = m_ID - 1;
        ch = 'B';
    }
    char* buf = new char[buflen + 1];
    memset(buf, 0,buflen + 1);
    memset(buf, ch, buflen);

    m_num++;
    MsgHeader msg;
    msg.cmd = RELAY;
    msg.length = buflen;
    msg.para1 = m_num;
    msg.para2 = toClientId;

    sendPackage(msg, buf);

    delete [] buf;
    buf = NULL;
}

    void
ServerAgent::writeBack(bool result)
{
    if (!result)
    {
        ERROR_LOG("\nIn ServerAgent: "
                "writeBack(): write out error!\n");
    }
}

    int
ServerAgent::sendPackage(MsgHeader &header, const char *dataStr)
{
    uint32_t msgLength = header.length + sizeof(MsgHeader);
    char *sendBuf = new char[msgLength + 1];
    memset(sendBuf, 0, msgLength + 1);
    memcpy(sendBuf, &header, sizeof(MsgHeader));
    if (dataStr != NULL)
    {
        memcpy(sendBuf + sizeof(MsgHeader), dataStr, header.length);
    }

    if (this->writeToBuffer(sendBuf, msgLength) == FAILED)
    {
        DEBUG_LOG("\nIn ServerAgent: "
                "sendPackage(): write data error!\n");
        return FAILED;
    }

    return SUCCESSFUL;
}
