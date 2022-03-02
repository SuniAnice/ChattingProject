

#pragma once


#include "Session.h"
#include <string>
#include <vector>


using namespace std;


class ChattingRoom
{
public:
	string m_name;
	int m_maxPeople;
	vector< Session* > m_chatters;
};

