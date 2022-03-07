

#pragma once


#include <string>


namespace str
{
	namespace errormsg
	{
		const std::string WSASTARTUP = "WSAStartup Error";
		const std::string SELECT = "Select Error";
		const std::string ACCEPT = "Accept Error";
		const std::string RECV = "Recv Error";
		const std::string COMMAND = "잘못된 명령어 입력입니다.\r\n";
		const std::string SELFERROR = "대상이 자기 자신입니다.\r\n";
	}
	namespace msg
	{
		const std::string CLIENT_LOGON = "클라이언트 접속 : ";
		const std::string CLIENT_LOGOUT = "클라이언트 접속종료 : ";

		const std::string PLAYER_ENTERROOM = "님이 대화방에 입장했습니다.\r\n";
		const std::string PLAYER_EXITROOM = "님이 대화방에서 나갔습니다.\r\n";

		const std::string PLAYER_ENTERLOGINSCENE = "안녕하세요. 채팅 서버에 오신 것을 환영합니다.\r\n사용하실 닉네임을 입력해주세요.\r\n";

		const std::string PLAYER_USINGNICKNAME = "을 닉네임으로 사용합니다\r\n";
		const std::string PLAYER_NICKNAMEEXIST = "중복된 닉네임이 존재합니다\r\n";
		const std::string PLAYER_NICKNAMESIZEERROR = "닉네임은 2글자 이상, 8글자 이하로 해주세요.\r\n";

		const std::string PLAYER_FAILMAKEROOM_MAX = "방 만들기 명령어 : a [제한인원] [방제목] (인원수 2명 ~ 20명)\r\n";
		const std::string PLAYER_FAILMAKEROOM_NAME = "방 만들기 명령어 : a [제한인원] [방제목] (제목 글자수 2 ~ 20자)\r\n";

		const std::string PLAYER_ROOMISFULL = "방이 가득 찼습니다.\r\n";
		const std::string PLAYER_FAILENTERROOM = "방 입장 명령어 : j [방 번호] (방 입장에 실패했습니다.)\r\n";

		const std::string PRINT_ROOMLIST = "방 목록을 출력합니다.\r\n";
		const std::string PRINT_PLAYERLIST = "접속중인 플레이어들을 출력합니다.\r\n";

		const std::string PRINT_PLAYERINFO = "해당 플레이어의 정보를 출력합니다.\r\n";
		const std::string PRINT_PLAYERINFO_NAME = "닉네임 : ";
		const std::string PRINT_PLAYERINFO_IP = "	ip : ";
		const std::string PRINT_PLAYERINFO_LOCATION = "	위치 : ";
		const std::string PRINT_PLAYERINFO_LOBBY = "로비\r\n";
		const std::string PRINT_PLAYERINFO_ROOM = "번 채팅방 \r\n";
		const std::string PRINT_FAILPLAYERINFO = "플레이어 정보 명령어 : i [닉네임] (해당 닉네임이 존재하지 않습니다)\r\n";

		const std::string PRINT_ROOMINFO = "해당 방의 정보를 출력합니다.\r\n";
		const std::string PRINT_ROOMINFO_NAME = "이름 : ";
		const std::string PRINT_ROOMINFO_CAPACITY = "	정원 : ";
		const std::string PRINT_ROOMINFO_PLAYER = "참가자 : ";
		const std::string PRINT_ROOMINFO_TIME = "개설 시각 : ";
		const std::string PRINT_FAILROOMINFO = "방 정보 명령어 : p [방 번호] (해당 채팅방이 존재하지 않습니다)\r\n";

		const std::string INVITE = "님을 채팅방으로 초대합니다.\r\n";
		const std::string INVITE_FROM = "님께서 ";
		const std::string INVITE_TO = "번 채팅방으로 초대하셨습니다.\r\n";
		const std::string INVITE_FAIL = "초대 명령어 : w [닉네임] (해당 플레이어가 존재하지 않습니다)\r\n";

		const std::string WISPER_TO = "님에게 귓속말 : ";
		const std::string WISPER_FROM = "님의 귓속말 : ";
		const std::string WISPER_FAIL = "귓속말 명령어 : t [닉네임] [할 말] (해당 닉네임이 존재하지 않습니다)\r\n";

		const std::string INFO_COMMAND = "----------------------------------------------------\r\n명령어 목록을 출력합니다.\r\n----------------------------------------------------\r\n";
		const std::string INFO_COMMAND_ROOM = "플레이어 목록 :		L\r\n플레이어 정보 :		i [닉네임]\r\n귓속말 :		t [닉네임] [할 말]\r\n초대 :			w [닉네임]\r\n방 목록 :		o\r\n방 정보 :		p [방 번호]\r\n방 나가기 :		q\r\n종료 :			x\r\n";
		const std::string INFO_COMMAND_LOBBY = "방 만들기 :		a [인원수] [방제목]\r\n플레이어 목록 :		L\r\n플레이어 정보 :		i [닉네임]\r\n귓속말 :		t [닉네임] [할 말]\r\n방 목록 :		o\r\n방 정보 :		p [방 번호]\r\n방 입장 :		j\r\n종료 :			x\r\n";

		const std::string DEFAULT_ROOM = "명령어 보기(h) 나가기(X)\r\n";
		const std::string DEFAULT_LOBBY = "----------------------------------------------------\r\n로비에 오신 것을 환영합니다\r\n----------------------------------------------------\r\n명령어 보기(h) 나가기(X)\r\n";

	}

}
