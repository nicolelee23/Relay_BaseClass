/*************************************************************************
	> File Name: LogOffTask.h
	> Author: cooperz
	> Mail: zbzcsn@qq.com
	> Created Time: Sun 18 Oct 2015 07:17:24 PM CST
 ************************************************************************/

#ifndef _LOGOFFTASK_H
#define _LOGOFFTASK_H

#include "RelayBaseTask.h"

class LogOffTask:public RelayBaseTask
{
    public:
        LogOffTask(){};
        ~LogOffTask(){};

        void setClientAgentId(uint32_t id)
        {
            client_agent_id_ = id;
        }
        int goNext();
        void recvWorkItem(ThreadPoolWorkItem*);
    private:
        uint32_t client_agent_id_;
};

#endif
