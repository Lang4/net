#pragma once
#include <string>
#include "network.h"
class GatewayLogic{
public:
	REMOTE_CLASS(GatewayLogic);
	int logicID; // 逻辑节点编号
	int mapID; // 地图服务器编号
	/**
	 * 过滤器
	 */
	bool filter(void *cmd,unsigned int len)
	{
		// 检查1s内若消息量太大 则关掉连接

		return true;
	}
	/**
	 * 定时行为
	 */
	void tick()
	{
		// 若 ns内无消息前来 则断开连接
	}

	void doLogin(const std::string &name,const std::string& pwd)
	{
		// 处理登录
		//records.get(logicID,name,"GatewayLogic::backData",1000,"GatewayLogic::backDataTimeout");
	}

	/**
	 * 处理回调数据
	 */
	void backData(const UserData &data)
	{
		// 验证密码
		// 确定场景连接
		//maps.get(logicID,data.mapName,"GatewayLogic::intoScene");
	}
	/**
	 * 处理回调数据超时
	 */
	void backDataTimeout()
	{
		//断开连接
	}

	void intoScene()
	{
		//if (scene)
		{
			//remote::call(scene->getConnection(),"Scene::intoScene",data); 登录到场景节点
		}
	}
	void sceneInValid()
	{
		// 场景失效时 重新选取
		// 重走登录流程
	}
	REMOTE_FUNCTION_1(test,const std::string &)
	{
		printf("test\n");
	}
};