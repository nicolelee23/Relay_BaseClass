/*************************************************************************
	> File Name: RelayBaseTask.h
	> Author: cooperz
	> Mail: zbzcsn@qq.com
	> Created Time: Sun 18 Oct 2015 07:27:16 PM CST
 ************************************************************************/

#ifndef _RELAYBASETASK_H
#define _RELAYBASETASK_H

#include "comm/BaseTask.h"
#include <string>
#include <string.h>
using std::string;
const uint32_t WORKING = 1;
const uint32_t FINISHED = 2;

class RelayBaseTask:public BaseTask
{
    public:
        RelayBaseTask()
        {
            state_ = WORKING;
        }
        virtual ~RelayBaseTask(){};
        void setData(string data)
        {
            data_ = data;
        }
    protected:
        uint32_t state_;
        string data_;
};

#endif
