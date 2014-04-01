#include "MyServer.h"
#include "MyPool.h"
#include "network.h"
#include "MyClient.h"
#include "ManagerLogic.h"
/**
 * ÿ�����ӵĴ���
 */
class LogicConnection:public MyConnection{
public:
	void beInit()
	{
		this->registerLogics(&theLogics);
	}
	~LogicConnection()
	{
		printf("��ж����\n");
	}
};
/**
 * ÿ���ͻ���
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
 * ����ڵ�
 **/
class Server:public MyServer<LogicConnection>{
public:
	void beInit()
	{
		printf("������\n");
		
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
		// �����ڵ���Ϣ ���ڵ���Ϣ�ﵽ����
		// �㲥�ڵ���Ϣ �����ڵ����ɽ����ڵ�����
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