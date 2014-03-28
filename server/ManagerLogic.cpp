#include "ManagerLogic.h"
#include "db.h"
#include "Info.h"
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
	printf("dome\n");
}