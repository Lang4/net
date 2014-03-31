#include "ManagerLogic.h"
#include "Info.h"

CLASS_MAP(ManagerLogic)
{
	BIND_NET_FUNCTION(ManagerLogic,reqLogin);
	BIND_NET_FUNCTION(ManagerLogic,reqLogin1);
	BIND_NET_FUNCTION(ManagerLogic,reqLogin2);
	BIND_NET_FUNCTION(ManagerLogic,reqLogin3);
	BIND_NET_FUNCTION(ManagerLogic,reqLogin4);
}

void ManagerLogic::reqLogin(const PhysicNodeInfo &info)
{
	
}

void ManagerLogic::reqLogin1(const PhysicNodeInfo &info,int a)
{
	
}
void ManagerLogic::reqLogin2(const PhysicNodeInfo &info,int b,int c)
{
	
}
void ManagerLogic::reqLogin3(const PhysicNodeInfo &info,int b,int c,int d)
{
	
}
void ManagerLogic::reqLogin4(const PhysicNodeInfo &info,int b,int c,int d,int e)
{
	
}

CLASS_MAP(ManagerClient)
{
	BIND_NET_FUNCTION(ManagerClient,retStart);
}

void ManagerClient::retStart(const LogicNodeInfo &info)
{
	printf("ManagerClient::retStart %s\n",info.ip);
}