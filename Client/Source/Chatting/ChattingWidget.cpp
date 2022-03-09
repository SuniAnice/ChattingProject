// Fill out your copyright notice in the Description page of Project Settings.


#include "ChattingWidget.h"
#include <string>
#include <locale.h>

std::string wcs_to_mbs( std::wstring wstr )
{
	const wchar_t* str = wstr.c_str();
	char mbs[ BUFFER_SIZE ] = { 0 };
	mbstate_t shiftState = mbstate_t();
	setlocale(LC_ALL, "");
	wcsrtombs( mbs, &str, sizeof( mbs ), &shiftState );
	std::string ret = mbs;
	ret += "\r\n";
	return ret;
}

void UChattingWidget::SendChat( FString string )
{
	std::wstring wstr = *string;
	std::string str = std::move( wcs_to_mbs( wstr ) );
	m_manager->Send( str.c_str(), str.size() );
}