#pragma once
#include "Info.h"
#include "network.h"
#include "MyTick.h"
#include "MyServer.h"
/**
 * 管理服务器的服务逻辑
 */

class ManagerLogic:public Logic
{
public:
	REMOTE_CLASS(ManagerLogic);
	/**
	 * 向管理节点注册函数(函数唯一名字,函数,参数);
	 */
	REMOTE_FUNCTION_1(reqLogin,const PhysicNodeInfo &);

};

class ManagerClient:public Logic
{
public:
	REMOTE_CLASS(ManagerClient);
	/**
	 * 管理节点告知当前节点启动的服务类型
	 */
	REMOTE_FUNCTION_1(retStart,const LogicNodeInfo &); 
};

class ManagerTick:public MyTick{
public:
	ManagerTick():MyTick(1000){}
	/**
	 * 5s定时设备
	 */
	void tick()
	{
		// 遍历节点信息 若节点信息达到需求
		// 广播节点信息 各个节点自由建立节点连接
		printf("ManagerTick::tick\n");
	}
};