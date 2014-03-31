#include "start.h"
#include "MyTick.h"
Server server;
/**
 * 创建连接
 */
static const char*managerIP="127.0.0.1";
static unsigned short port = 1234;

void Server::beInit()
{
	printf("启动服务器\n");
	
	theTick.addTick(new ManagerTick());
}

void Client::beInit()
{
	if (this->checkValid())
	{
		PhysicNodeInfo info;
		ManagerLogic::R_reqLogin(this,info);
		printf("ManagerLogic::R_reqLogin\n");
	}
}
void Server::beFinal()
{

}
void init()
{
	PhysicNodeInfo info;
	ManagerLogic::R_reqLogin(URL<Client>::Get(managerIP,1235),info);
}
/**
 * ./Service manager | 启动manager
 * ./Service 
 * FLServer 1
 * Gateway 1 ~10
 * Scene 1~10
 * RecordCenter 1
 * RecordServer 1~10
 * count 群组最低进程数
 * *************************************************
 * 节点数据表
 * UNIQUEID,服务器IP,服务器PORT,运行状态,节点服务类型
 * **************************************************
 * 地图节点表
 ****************************************************
 * UNQIUEID,名字,编号,节点唯一编号,人数,运行状态，地图类型
 ****************************************************
 * 地图XML数据
 * 名字 大小 容纳人数
 ****************************************************
 * Manager 设定运行各个节点 
 **/

int main(int args,char* argv[])
{
	theLogics.setup();
	if (args == 1) // 如果是管理节点
	{
		server.init(managerIP,port);
		server.go();
	}
	else
		init();

}