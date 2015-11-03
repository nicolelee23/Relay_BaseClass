/*************************************************************************
	> File Name: LogInTask.h
	> Author: cooperz
	> Mail: zbzcsn@qq.com
	> Created Time: Sun 18 Oct 2015 07:17:24 PM CST
 ************************************************************************/

#ifndef _LOGINTASK_H
#define _LOGINTASK_H

#include "RelayBaseTask.h"

#define READMESSAGE 0
#define SENDMESSAGE 1
#define FINISHLOGIN 2


class LogInTask:public RelayBaseTask
{
    public:
        LogInTask()
        {
            state_ = READMESSAGE;
            message_buf_ = NULL;
            buf_len_ = 0;
        };
        ~LogInTask(){};
        void setClientAgentId(uint32_t id)
        {
            client_agent_id_ = id;
        }

        int goNext();
        void recvWorkItem(ThreadPoolWorkItem*);
    private:
        char* message_buf_;
        uint32_t buf_len_;
        uint32_t client_agent_id_;
};

#endif
