

#pragma once


#include "Session.h"
#include <vector>
#include <string>


void BroadcastMessage( const vector< Session* >& container, const string& message );

bool ProcessCommand( Session& sock );
