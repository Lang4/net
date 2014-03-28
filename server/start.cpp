#include "start.h"
#include "MyTick.h"
Server server;
/**
 * ��������
 */
static const char*managerIP="127.0.0.1";
static unsigned short port = 1234;

void Server::beInit()
{
	printf("����������\n");
	
	theTick.addTick(new ManagerTick());
}
void Logics::init()
{
	BIND_NET_FUNCTION(ManagerLogic,reqLogin);
	BIND_NET_FUNCTION(GatewayLogic,test);
	BIND_NET_FUNCTION(MyConnection,recvTick);
}
void Logics::dispatch(MyConnection *connection,const std::string &className,char *cmd,unsigned int len)
{
	LogicConnection* logic = (LogicConnection*) connection;
	if (className == ManagerLogic::C_NAME())
	{
		remote::call(&logic->manager,cmd,len);
	}
	else if (className == GatewayLogic::C_NAME())
	{
		remote::call(&logic->gateway,cmd,len);
	}
}
void Server::beFinal()
{

}
void init()
{
	//remote::call(theClients.get(managerIP,managerPort),"ManagerLogic::reqLogin");
	//TimeTick::getMe().start();
	//join();
}
/**
 * ./Service manager | ����manager
 * ./Service 
 * FLServer 1
 * Gateway 1 ~10
 * Scene 1~10
 * RecordCenter 1
 * RecordServer 1~10
 * count Ⱥ����ͽ�����
 * *************************************************
 * �ڵ����ݱ�
 * UNIQUEID,������IP,������PORT,����״̬,�ڵ��������
 * **************************************************
 * ��ͼ�ڵ��
 ****************************************************
 * UNQIUEID,����,���,�ڵ�Ψһ���,����,����״̬����ͼ����
 ****************************************************
 * ��ͼXML����
 * ���� ��С ��������
 ****************************************************
 * Manager �趨���и����ڵ� 
 **/

int main()
{
	theLogics.setup();
	if (1) // ����ǹ���ڵ�
	{
		server.init(managerIP,port);
		server.go();
	}
	else
		init();
}