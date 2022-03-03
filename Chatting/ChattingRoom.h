

#pragma once


#include <string>
#include <vector>


class Session;

class ChattingRoom
{
private:
	std::string m_name;
	int m_maxPeople;
	std::vector< Session* > m_chatters;
public:
	std::string& GetName();
	int GetMaxPeople() const;
	std::vector< Session* >& GetChatters();

	void SetName( const std::string& name );
	void SetMaxPeople( int max );
};

