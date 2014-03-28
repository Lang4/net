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
		
	}
	virtual void doCmd(void*cmd,unsigned int len) 
	{
		
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
		return false;
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
	}
	void beFinal()
	{
	
	}
};

/** 
 * �߼�ת����
 * className ָ����Э�鸸�� 
 **/
void Logics::dispatch(MyConnection *connection,const std::string &className,char *cmd,unsigned int len)
{

}

void Logics::init()
{
	BIND_NET_FUNCTION(MyConnection,recvTick);
}
int main()
{
	theLogics.setup();
	
	Server server;
	server.init("127.0.0.1",1235);
	
	server.go();
}