/*************************************************************************
	> File Name: LogOffWorkItem.h
	> Author: cooperz
	> Mail: zbzcsn@qq.com
	> Created Time: Sun 18 Oct 2015 08:41:03 PM CST
 ************************************************************************/

#ifndef _LOGOFFWORKITEM_H
#define _LOGOFFWORKITEM_H

#include "sys/ThreadPoolWorkItem.h"
#include <string>
#include <string.h>
using std::string;
class LogOffWorkItem : public ThreadPoolWorkItem
{
    public:
        void setData(string data)
        {
            data_ = data;
        }
        void setClientAgentId(uint32_t clientAgentId)
        {
            client_agent_id_ = clientAgentId;
        }
        int process();
    private:
        uint32_t client_agent_id_;
        string data_;
};
#endif
