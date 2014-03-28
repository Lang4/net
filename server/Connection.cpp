#include "Connection.h"
#include "MyClient.h"
#include "Info.h"
#include "MyTick.h"
#include "start.h"
void LogicConnection::beInit()
{
	printf("连接来了\n");
	this->registerLogics(&theLogics);
	PhysicNodeInfo info;

	//ManagerLogic::R_reqLogin(URL("127.0.0.1",1234),info);

	//gateway.R_test(URL("127.0.0.1",1234),"hello,world");

	theTick.addTick(this);

}
bool LogicConnection::beFinal()
{
	theTick.removeTick(this);
	return false;
}