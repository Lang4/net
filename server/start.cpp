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

int main(int args,char* argv[])
{
	theLogics.setup();
	if (args == 1) // ����ǹ���ڵ�
	{
		server.init(managerIP,port);
		server.go();
	}
	else
		init();

}