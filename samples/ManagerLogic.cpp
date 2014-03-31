#include "ManagerLogic.h"
#include "Info.h"

CLASS_MAP(ManagerLogic)
{
	BIND_NET_FUNCTION(ManagerLogic,reqLogin);
}

void ManagerLogic::reqLogin(const PhysicNodeInfo &info)
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