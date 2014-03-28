/**
 * 群组容错机制说明
 * 除了网关 或 用户网关 宕机，其他节点的宕机都可以屏蔽重启 以及 线上热更新 也即不需重启整个群组 完成对服务器的更新
 **/
/**
 * 群组逻辑的开发
 * 群组逻辑屏蔽了网络层 可以同时使用windows , linux 下开发测试, 完成了对jygame的重构与扩展 并在可以windows 下运行
 * 尝试在不改变flash 端的情况下 对服务器进行重构 并支撑到场景PK 同时对角色数据进行动态迁移
 */
#pragma once
#include "MyServer.h"
#include "Connection.h"
class Server:public MyServer<LogicConnection>{
public:
	void beInit();
	void beFinal();
};
extern Server server;
