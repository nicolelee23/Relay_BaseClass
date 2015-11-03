/*************************************************************************
  > File Name: LogOffWorkItem.cpp
  > Author: cooperz
  > Mail: zbzcsn@qq.com
  > Created Time: Sun 18 Oct 2015 08:44:40 PM CST
 ************************************************************************/

#include "LogOffWorkItem.h"
#include "comm/Error.h"
#include "CToSMapManager/CToSMapManager.h"

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
using namespace std;

#define MODE S_IRUSR|S_IWUSR|S_IXUSR

 string PREFIX = "OfflineMessage/";
 int charLen = 5;


int LogOffWorkItem::process()
{
    uint32_t clientId = (CToSMapManager::getInstance())->findClientId(client_agent_id_);
    char* str = new char[charLen];
    memset(str, 0, charLen);
    sprintf(str, "%d", clientId);
    string fileName = PREFIX + str;
    
    int fileFd = 0;
    if(FAILED == (fileFd = open(fileName.c_str(), O_WRONLY|O_CREAT|O_TRUNC, MODE)))
    {
        cout<<"in LogOffWorkItem::process:open"<<endl;
        return FAILED;
    }
    if(FAILED == write(fileFd, data_.c_str(), data_.length()))
    {
        cout<<"in LogOffWorkItem::process:write"<<endl;
        return FAILED;
    }
    delete []str;
    str = NULL;
    cout<<"saved file:"<<fileName<<endl; 
    return SUCCESSFUL;
}
