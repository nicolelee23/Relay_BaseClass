/*************************************************************************
  > File Name: LogInTask.cpp
  > Author: cooperz
  > Mail: zbzcsn@qq.com
  > Created Time: Sun 18 Oct 2015 07:23:49 PM CST
 ************************************************************************/

#include "LogInTask.h"
#include "comm/Error.h"
#include "sys/ThreadPoolDispatcher.h"
#include "LogInWorkItem.h"
#include "comm/BaseHeader.h"
#include "comm/AgentManager.h"
#include "comm/TaskManager.h"
#include "../ClientAgent.h"
#include "Protocol/Protocol.h"

int LogInTask::goNext()
{
    switch(state_)
    {
        case READMESSAGE:
            {
                LogInWorkItem* pWorkItem = new LogInWorkItem();
                pWorkItem->setData(data_);
                pWorkItem->setTaskID(getID());
                g_pDispatcher->postRequest(pWorkItem);
                break;
            }
        case SENDMESSAGE:
            {
                MsgHeader msg;
                memset(&msg, 0, sizeof(msg));
                msg.cmd = READOFFLINENEWS;
                msg.length = buf_len_;
                dynamic_cast<ClientAgent*>((AgentManager::getInstance())->get(client_agent_id_))->sendPackage(msg, message_buf_);
                if(message_buf_ != NULL)
                {
                delete []message_buf_;
                message_buf_ = NULL;
                }
                state_ = FINISHLOGIN;
                goNext();
                break;
            }
        case FINISHLOGIN:
            {
                TaskManager::getInstance()->recycle(getID());
                break;
            }
    }
    return SUCCESSFUL;
}

void LogInTask::recvWorkItem(ThreadPoolWorkItem* pWorkItem)
{
    message_buf_ = dynamic_cast<LogInWorkItem*>(pWorkItem)->getMessageBuf();
    buf_len_ = dynamic_cast<LogInWorkItem*>(pWorkItem)->getMessageBufLen();
    
    if(pWorkItem != NULL)
    {
        delete pWorkItem;
        pWorkItem = NULL;
    }
    state_ = SENDMESSAGE;
    goNext();
}
