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
	ManagerLogic manager;
	GatewayLogic gateway;
	void beInit();
	bool beFinal();
};
