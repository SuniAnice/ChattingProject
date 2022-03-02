

#pragma once


#include <string>
#include <vector>


using namespace std;


class Session;

class ChattingRoom
{
public:
	string m_name;
	int m_maxPeople = 0;
	vector< Session* > m_chatters;
};

