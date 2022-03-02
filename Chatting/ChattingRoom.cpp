

#include "ChattingRoom.h"

string& ChattingRoom::GetName()
{
    return m_name;
}

int ChattingRoom::GetMaxPeople() const
{
    return m_maxPeople;
}

vector<Session*>& ChattingRoom::GetChatters()
{
    return m_chatters;
}

void ChattingRoom::SetName( const string& name )
{
    m_name = name;
}

void ChattingRoom::SetMaxPeople( int max )
{
    m_maxPeople = max;
}
