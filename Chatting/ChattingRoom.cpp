

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

void ChattingRoom::SetName( const std::string& name )
{
    m_name = name;
}

void ChattingRoom::SetMaxPeople( int max )
{
    m_maxPeople = max;
}
