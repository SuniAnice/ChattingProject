

#pragma once


#include <string>
#include <vector>
#include <ctime>


class Session;

class ChattingRoom
{
private:
	std::string m_name;
	int m_maxPeople;
	std::vector< Session* > m_chatters;
	time_t m_time;
public:
	std::string& GetName();
	int GetMaxPeople() const;
	std::vector< Session* >& GetChatters();
	time_t GetTime();

	void SetName( const std::string& name );
	void SetMaxPeople( int max );
	void SetTime( time_t time );
};

