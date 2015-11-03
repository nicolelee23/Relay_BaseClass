/*************************************************************************
  > File Name: LogInWorkItem.cpp
  > Author: cooperz
  > Mail: zbzcsn@qq.com
  > Created Time: Sun 18 Oct 2015 08:44:40 PM CST
 ************************************************************************/

#include "LogInWorkItem.h"
#include "comm/Error.h"
#include "CToSMapManager/CToSMapManager.h"

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;

#define MODE S_IRUSR|S_IWUSR|S_IXUSR


int LogInWorkItem::process()
{
                      struct stat  buf;
                      stat(data_.c_str(), &buf);
                      int fileSize = buf.st_size;
                      message_buf_ = new char [fileSize];
                      buf_len_ = fileSize;
                      int fileFd;
                      if(FAILED == (fileFd = open(data_.c_str(), O_RDONLY)))
                      {
                          cout<<"in LogInWorkItem::process:open offlinefile error"<<endl;
                          return FAILED;
                      }
                      if(fileSize != read(fileFd, message_buf_, fileSize))
                      {
                          cout<<"in LogInWorkItem::process:read offlinefile error"<<endl;
                          return FAILED;
                      }

    return SUCCESSFUL;
}
