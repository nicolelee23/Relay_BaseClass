/*
 * =====================================================================================
 *
 *       Filename:  ClientAgent.cpp
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

#include "ClientAgent.h"

#include "Protocol/Protocol.h"
#include "CToSMapManager/CToSMapManager.h"
#include "common/comm/AgentManager.h"
#include "common/comm/TaskManager.h"
#include "common/log/log.h"

#include "Task/LogOffTask.h"
#include "Task/LogInTask.h"

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
using namespace std;

extern  string PREFIX;
extern  int charLen;

ClientAgent::ClientAgent()
{

}

ClientAgent::ClientAgent(const SocketAddress &addr):
    TCPAgent(addr)
{

}

ClientAgent::ClientAgent(const TCPSocket &sock, const SocketAddress &addr):
    TCPAgent(sock,addr)
{

}

ClientAgent::~ClientAgent()
{
}

    void
ClientAgent::readBack(InReq &req)
{
    MsgHeader msg;
    memset(&msg, 0, sizeof(msg));
    if(req.m_msgHeader.cmd == REGISTER)
    {
        msg.cmd = REGISTERACK;
        int buflen = req.m_msgHeader.length;
        msg.length = buflen;
        char* buf = new char[buflen + 1];
        memset(buf, 0, buflen + 1);
        memset(buf, '#', buflen);
        dynamic_cast<ClientAgent*>((AgentManager::getInstance())->get(getID()))->sendPackage(msg, buf);
        if(buf != NULL)
        {
            delete buf;
            buf = NULL;
        }

        bool isExist = CToSMapManager::getInstance()->add(req.m_msgHeader.para2, getID());
        if(isExist)
        {
            uint32_t clientId = req.m_msgHeader.para2;
            char* fileName = new char [charLen];
            memset(fileName, 0, charLen);
            sprintf(fileName, "%d", clientId);
            string filePath = PREFIX + fileName;            
                if(!access(filePath.c_str(), F_OK))
                  {
                      cout<<"read offline message:"<<endl;
                      LogInTask* pTask = TaskManager::getInstance()->create<LogInTask>();
                      pTask->setData(filePath);
                      pTask->setClientAgentId(getID());
                      pTask->goNext();
                  }

        }

        return ;
    }
    uint32_t toClientId = req.m_msgHeader.para2;
    uint32_t toClientAgentId = (CToSMapManager::getInstance())->findClientAgentId(toClientId);

    if(!toClientAgentId)
    {
        ERROR_LOG("in ClientAgent::readBack:not exist toClientAgentId");
        return ;
    }

    msg.cmd = RELAYACK;
    msg.length = req.m_msgHeader.length;
    //其他字段暂不需要
    dynamic_cast<ClientAgent*>((AgentManager::getInstance())->get(toClientAgentId))->sendPackage(msg, req.ioBuf);
}

    void
ClientAgent::writeBack(bool result)
{
    if (!result)
    {
        ERROR_LOG("\nIn ClientAgent: "
                "writeBack(): write out error!\n");
    }
}

    int
ClientAgent::sendPackage(MsgHeader &header, const char* dataStr)
{
    uint32_t msgLength = header.length + sizeof(MsgHeader);
    char *sendBuf = new  char[msgLength + 1];
    memset(sendBuf, 0, msgLength + 1);
    memcpy(sendBuf, &header, sizeof(MsgHeader));
    if(dataStr != NULL)
    {
        memcpy(sendBuf + sizeof(MsgHeader), dataStr, header.length);
    }
    if (this->writeToBuffer(sendBuf, msgLength) == FAILED)
    {
        DEBUG_LOG("\nIn ClientAgent: "
                "sendPackage(): write data error!\n");
        return FAILED;
    }

    return SUCCESSFUL;
}

int ClientAgent::recvData()
{
    int readN = 0;
    if ( FAILED == (readN = this->m_buffer.readTCP() ) )
    {
        ERROR_LOG( "ClientAgent::readData():m_buffer.read()" );
        ERROR_LOG( 
                "opposit: %s " , m_addr.getIP()); //pxy
        cout<<getID()<<":"<<endl;
        perror("recvData");
        //这里应该是readN返回值为0，即说明客户端关闭，但是由于下层read=0返回的是-1，所以在不改变基类的情况下，就一概而论了
        if(errno == 104)//Connection reset by peer
        {
            if ( this->m_buffer.getSendBufferLength() != 0 )
            {
                cout<<"exist offline message"<<endl;
                /*//这里还是放在BufferManager直接获得离线消息比较好，即直接在BufferManager里面把list里面的数据转换成消息buffer
                 *原因：1）都是主线程里；2）buffer,比如返回的list里面的m_pCompleteBuffer与原来的list里面的指向的是同一地址空间；既然BufferManager里面有回收。PS：这里有内存泄露，因为可能BufferManager还没有回收就被析构了
                list<Iov> messageList = this->m_buffer.getSendIovList();//即结构体的赋值，里指针指向同一地址
                list<Iov> ::iterator it;
                string data;
                for(it = messageList.begin(); it != messageList.end();)
                {
                    data.append((*it).m_Iovec.iov_base);
                    messageList.erase(it++);//会回收栈上的Iov结构体,即会执行Iov的析构函数
                }
                */
                string data;
                this->m_buffer.getSendIovListBuffer(data);
                LogOffTask* pTask = (TaskManager::getInstance()->create<LogOffTask>());
                pTask->setData(data);
                pTask->setClientAgentId(getID());
                pTask->goNext();
            }
        }
        return FAILED;
    }

    return SUCCESSFUL;
}

int ClientAgent::sendData( void )
{
    if ( this->m_buffer.write() < 0 )
    {
        ERROR_LOG( "ClientAgent::writeData():m_buffer.write(TCPSocket)" );
        
        cout<<getID()<<":"<<endl;
        perror("sendData");

        if(errno == 32)//broken pipe
        {
            if ( this->m_buffer.getSendBufferLength() != 0 )
            {
                cout<<"exist offline message"<<endl;
                string data;
                this->m_buffer.getSendIovListBuffer(data);
                LogOffTask* pTask = (TaskManager::getInstance()->create<LogOffTask>());
                pTask->setData(data);
                pTask->setClientAgentId(getID());
                pTask->goNext();
            }
        }
        return FAILED;
    }
    if ( this->m_buffer.getSendBufferLength() == 0 )
    {
        if ( this->m_epollEvent.closeWevent() < 0 )
        {
            INFO_LOG( "this->m_epollEvent.closeWevent error" );
        }
    }
}
