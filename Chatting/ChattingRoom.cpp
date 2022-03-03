

#include "ChattingRoom.h"

std::string& ChattingRoom::GetName()
{
    return m_name;
}

int ChattingRoom::GetMaxPeople() const
{
    return m_maxPeople;
}

std::vector<Session*>& ChattingRoom::GetChatters()
{
    return m_chatters;
}

time_t ChattingRoom::GetTime()
{
    return m_time;
}

void ChattingRoom::SetName( const std::string& name )
{
    m_name = name;
}

void ChattingRoom::SetMaxPeople( int max )
{
    m_maxPeople = max;
}

void ChattingRoom::SetTime( time_t time )
{
    m_time = time;
}
