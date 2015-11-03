/*************************************************************************
	> File Name: LogOffTask.cpp
	> Author: cooperz
	> Mail: zbzcsn@qq.com
	> Created Time: Sun 18 Oct 2015 07:23:49 PM CST
 ************************************************************************/

#include "LogOffTask.h"
#include "comm/Error.h"
#include "comm/TaskManager.h"
#include "sys/ThreadPoolDispatcher.h"
#include "LogOffWorkItem.h"

int LogOffTask::goNext()
{
    switch(state_)
    {
        case WORKING:
            {
                LogOffWorkItem* pWorkItem = new LogOffWorkItem();
                pWorkItem->setData(data_);
                pWorkItem->setTaskID(getID());
                pWorkItem->setClientAgentId(client_agent_id_);
                g_pDispatcher->postRequest(pWorkItem);
            break;
            }
        case FINISHED:
            {
                TaskManager::getInstance()->recycle(getID());
                break;
            }
    }
    return SUCCESSFUL;
}

void LogOffTask::recvWorkItem(ThreadPoolWorkItem* pWorkItem)
{
    if(pWorkItem != NULL)
    {
        delete pWorkItem;
        pWorkItem = NULL;
    }
    state_ = FINISHED;
    goNext();
}
