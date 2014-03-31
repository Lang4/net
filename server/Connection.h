#pragma once

/**
 * 管理获取连接 通过名字 通过ID 通过 URL
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
		printf("我删除了\n");
	}
};
