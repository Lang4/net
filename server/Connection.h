#pragma once

/**
 * �����ȡ���� ͨ������ ͨ��ID ͨ�� URL
 **/
#include "ManagerLogic.h"
#include "GatewayLogic.h"
#include "MyConnection.h"
class LogicConnection:public MyConnection{
public:
	LogicConnection(){
	
	}
	void beInit();
	bool beFinal();
	~LogicConnection()
	{
		printf("��ɾ����\n");
	}
};
