/*************************************************************************
  > File Name: CToSMapManager.cpp
  > Author: cooperz
  > Mail: zbzcsn@qq.com
  > Created Time: Sat 17 Oct 2015 11:11:13 AM CST
 ************************************************************************/

#include "CToSMapManager.h"
#include "comm/Error.h"

bool CToSMapManager::add(uint32_t clientId, uint32_t clientAgentId)
{
    if(c_to_s_map_.find(clientId) == c_to_s_map_.end())
    {
        c_to_s_map_.insert(pair<uint32_t, uint32_t>(clientId, clientAgentId));
        return false;
    }
    else
    {
        c_to_s_map_[clientId] = clientAgentId;
        return true;
    }
}

uint32_t CToSMapManager::findClientAgentId(uint32_t clientId)
{
    if(c_to_s_map_.find(clientId) == c_to_s_map_.end())
    {
        return 0;
    }
    return c_to_s_map_[clientId];
}

uint32_t CToSMapManager::findClientId(uint32_t clientAgentId)
{
    map<uint32_t, uint32_t> :: iterator it;
    for(it = c_to_s_map_.begin(); it != c_to_s_map_.end(); ++it)
    {
        if(it->second == clientAgentId)
        {
            return it->first;
        }
    }
    return 0;
}
int CToSMapManager::remove(uint32_t clientAgentId)
{
    uint32_t clientId = 0;
    if(0 != (clientId = findClientId(clientAgentId)))
    {
    c_to_s_map_[clientId] = 0;
    return SUCCESSFUL;
    }
    else
       {
           return FAILED;
       }
}


