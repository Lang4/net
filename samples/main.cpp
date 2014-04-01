#include "MyServer.h"
#include "MyPool.h"
#include "network.h"
#include "MyClient.h"
#include "ManagerLogic.h"
/**
 * 每个连接的处理
 */
class LogicConnection:public MyConnection{
public:
	void beInit()
	{
		this->registerLogics(&theLogics);
	}
	~LogicConnection()
	{
		printf("我卸载啦\n");
	}
};
/**
 * 每个客户端
 */
class Client:public MyClient{
public:
	Client(const char *ip,unsigned short port):MyClient(ip,port){
		theTick.addTick(this);
		this->registerLogics(&theLogics);
	}
	void beInit()
	{
		
	}
	bool beFinal()
	{
		socket = -1;
		return true;
	}
};
/** 
 * 服务节点
 **/
class Server:public MyServer<LogicConnection>{
public:
	void beInit()
	{
		printf("连接她\n");
		
		PhysicNodeInfo info;
		ManagerLogic::R_reqLogin(URL<Client>::Get("127.0.0.1",1234),info);

		theTick.addTick(new ManagerTick());
	}
	void beFinal()
	{
	
	}
};

void ManagerTick::tick()
{
	{
		// 遍历节点信息 若节点信息达到需求
		// 广播节点信息 各个节点自由建立节点连接
		printf("ManagerTick::tick\n");
		PhysicNodeInfo info;
		//ManagerLogic::R_reqLogin(URL<Client>::Get("127.0.0.1",1234),info);
	}
}
int main(int argc,char* args[])
{
	theLogics.setup();
	if (argc == 2)
	{
		Server server;
		server.init("127.0.0.1",atoi(args[1]));
		
		server.go();
	}
}