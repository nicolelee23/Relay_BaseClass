/*************************************************************************
	> File Name: LogInWorkItem.h
	> Author: cooperz
	> Mail: zbzcsn@qq.com
	> Created Time: Sun 18 Oct 2015 08:41:03 PM CST
 ************************************************************************/

#ifndef _LOGINWORKITEM_H
#define _LOGINWORKITEM_H

#include "sys/ThreadPoolWorkItem.h"
#include <string>
#include <string.h>
using std::string;

class LogInWorkItem : public ThreadPoolWorkItem
{
    public:
        LogInWorkItem():message_buf_(NULL){}
        ~LogInWorkItem(){}
        void setData(string data)
        {
            data_ = data;
        }
        char* getMessageBuf()
        {
            return message_buf_;
        }
        uint32_t getMessageBufLen()
        {
            return buf_len_;
        }
        int process();
    private:
        string data_;
        char* message_buf_;
        uint32_t buf_len_;
};
#endif
