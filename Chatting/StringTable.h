

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
		const std::string COMMAND = "�߸��� ��ɾ� �Է��Դϴ�.\r\n";
		const std::string SELFERROR = "����� �ڱ� �ڽ��Դϴ�.\r\n";
	}
	namespace msg
	{
		const std::string CLIENT_LOGON = "Ŭ���̾�Ʈ ���� : ";
		const std::string CLIENT_LOGOUT = "Ŭ���̾�Ʈ �������� : ";

		const std::string PLAYER_ENTERROOM = "���� ��ȭ�濡 �����߽��ϴ�.\r\n";
		const std::string PLAYER_EXITROOM = "���� ��ȭ�濡�� �������ϴ�.\r\n";

		const std::string PLAYER_ENTERLOGINSCENE = "�ȳ��ϼ���. ä�� ������ ���� ���� ȯ���մϴ�.\r\n����Ͻ� �г����� �Է����ּ���.\r\n";

		const std::string PLAYER_USINGNICKNAME = "�� �г������� ����մϴ�\r\n";
		const std::string PLAYER_NICKNAMEEXIST = "�ߺ��� �г����� �����մϴ�\r\n";
		const std::string PLAYER_NICKNAMESIZEERROR = "�г����� 2���� �̻�, 8���� ���Ϸ� ���ּ���.\r\n";

		const std::string PLAYER_FAILMAKEROOM_MAX = "�� ����� ��ɾ� : a [�����ο�] [������] (�ο��� 2�� ~ 20��)\r\n";
		const std::string PLAYER_FAILMAKEROOM_NAME = "�� ����� ��ɾ� : a [�����ο�] [������] (���� ���ڼ� 2 ~ 20��)\r\n";

		const std::string PLAYER_ROOMISFULL = "���� ���� á���ϴ�.\r\n";
		const std::string PLAYER_FAILENTERROOM = "�� ���� ��ɾ� : j [�� ��ȣ] (�� ���忡 �����߽��ϴ�.)\r\n";

		const std::string PRINT_ROOMLIST = "�� ����� ����մϴ�.\r\n";
		const std::string PRINT_PLAYERLIST = "�������� �÷��̾���� ����մϴ�.\r\n";

		const std::string PRINT_PLAYERINFO = "�ش� �÷��̾��� ������ ����մϴ�.\r\n";
		const std::string PRINT_PLAYERINFO_NAME = "�г��� : ";
		const std::string PRINT_PLAYERINFO_IP = "	ip : ";
		const std::string PRINT_PLAYERINFO_LOCATION = "	��ġ : ";
		const std::string PRINT_PLAYERINFO_LOBBY = "�κ�\r\n";
		const std::string PRINT_PLAYERINFO_ROOM = "�� ä�ù� \r\n";
		const std::string PRINT_FAILPLAYERINFO = "�÷��̾� ���� ��ɾ� : i [�г���] (�ش� �г����� �������� �ʽ��ϴ�)\r\n";

		const std::string PRINT_ROOMINFO = "�ش� ���� ������ ����մϴ�.\r\n";
		const std::string PRINT_ROOMINFO_NAME = "�̸� : ";
		const std::string PRINT_ROOMINFO_CAPACITY = "	���� : ";
		const std::string PRINT_ROOMINFO_PLAYER = "������ : ";
		const std::string PRINT_ROOMINFO_TIME = "���� �ð� : ";
		const std::string PRINT_FAILROOMINFO = "�� ���� ��ɾ� : p [�� ��ȣ] (�ش� ä�ù��� �������� �ʽ��ϴ�)\r\n";

		const std::string INVITE = "���� ä�ù����� �ʴ��մϴ�.\r\n";
		const std::string INVITE_FROM = "�Բ��� ";
		const std::string INVITE_TO = "�� ä�ù����� �ʴ��ϼ̽��ϴ�.\r\n";
		const std::string INVITE_FAIL = "�ʴ� ��ɾ� : w [�г���] (�ش� �÷��̾ �������� �ʽ��ϴ�)\r\n";

		const std::string WISPER_TO = "�Կ��� �ӼӸ� : ";
		const std::string WISPER_FROM = "���� �ӼӸ� : ";
		const std::string WISPER_FAIL = "�ӼӸ� ��ɾ� : t [�г���] [�� ��] (�ش� �г����� �������� �ʽ��ϴ�)\r\n";

		const std::string INFO_COMMAND = "----------------------------------------------------\r\n��ɾ� ����� ����մϴ�.\r\n----------------------------------------------------\r\n";
		const std::string INFO_COMMAND_ROOM = "�÷��̾� ��� :		L\r\n�÷��̾� ���� :		i [�г���]\r\n�ӼӸ� :		t [�г���] [�� ��]\r\n�ʴ� :			w [�г���]\r\n�� ��� :		o\r\n�� ���� :		p [�� ��ȣ]\r\n�� ������ :		q\r\n���� :			x\r\n";
		const std::string INFO_COMMAND_LOBBY = "�� ����� :		a [�ο���] [������]\r\n�÷��̾� ��� :		L\r\n�÷��̾� ���� :		i [�г���]\r\n�ӼӸ� :		t [�г���] [�� ��]\r\n�� ��� :		o\r\n�� ���� :		p [�� ��ȣ]\r\n�� ���� :		j\r\n���� :			x\r\n";

		const std::string DEFAULT_ROOM = "��ɾ� ����(h) ������(X)\r\n";
		const std::string DEFAULT_LOBBY = "----------------------------------------------------\r\n�κ� ���� ���� ȯ���մϴ�\r\n----------------------------------------------------\r\n��ɾ� ����(h) ������(X)\r\n";

	}

}
