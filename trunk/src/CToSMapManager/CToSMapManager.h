/*************************************************************************
	> File Name: CToSMapManager.h
	> Author: cooperz
	> Mail: zbzcsn@qq.com
	> Created Time: Sat 17 Oct 2015 11:05:31 AM CST
 ************************************************************************/

#ifndef _CTOSMAPMANAGER_H
#define _CTOSMAPMANAGER_H

#include <stdint.h>
#include "sys/Singleton.h"
#include <map>

using std::map;
using std::pair;

class CToSMapManager:public Singleton<CToSMapManager>
{
    friend class Singleton<CToSMapManager>;
    public:

    bool add(uint32_t, uint32_t);
    uint32_t findClientAgentId(uint32_t);
    uint32_t findClientId(uint32_t);
    int remove(uint32_t);

    private:
    
    CToSMapManager(){}
    ~CToSMapManager(){}

    map<uint32_t, uint32_t> c_to_s_map_;
};

#endif
