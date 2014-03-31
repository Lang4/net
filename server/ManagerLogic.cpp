#include "ManagerLogic.h"
#include "db.h"
#include "Info.h"
#include "start.h"
class PhysicNodeInfo_Table:public db::DBTable{
public:
	LogicNodeInfo info;
	std::string ip;
	DEC_TABLE
};

TABLE_BEGIN(PhysicNodeInfo_Table)
	FIELD(info.nodeType);
	FIELD(info.port);
	FIELD(ip);
TABLE_END

CLASS_MAP(ManagerLogic)
{
	BIND_NET_FUNCTION(ManagerLogic,reqLogin);
}

void ManagerLogic::reqLogin(const PhysicNodeInfo &info)
{
	db::DB db;
	db.init("manager.info");
	PhysicNodeInfo_Table *nodeInfo = NULL;
	if (db.create(nodeInfo))
	{
		if (!nodeInfo->load(db,"where `ip`='127.0.0.1' and `info.port`='50'"))
		{
			nodeInfo->ip = "127.0.0.1";
			nodeInfo->info.port = 50;
			nodeInfo->save(db);
		}
		delete nodeInfo;
	}
	db.destroy();
	LogicNodeInfo arg1;
	strncpy(arg1.ip,"127.0.0.1",32);
	arg1.port = 2353;
	ManagerClient::R_retStart(this->getConnection(),arg1);
	printf("ManagerLogic::reqLogin\n");
}

CLASS_MAP(ManagerClient)
{
	BIND_NET_FUNCTION(ManagerClient,retStart);
}

void ManagerClient::retStart(const LogicNodeInfo &info)
{
	server.init(info.ip,info.port);
}