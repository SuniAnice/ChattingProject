

#pragma once


#include <string>
#include <vector>


using namespace std;


class Session;

class ChattingRoom
{
private:
	string m_name;
	int m_maxPeople;
	vector< Session* > m_chatters;
public:
	string& GetName();
	int GetMaxPeople() const;
	vector< Session* >& GetChatters();

	void SetName( const string& name );
	void SetMaxPeople( int max );
};

