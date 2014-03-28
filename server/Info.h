#pragma once

#pragma pack(1)

struct PhysicNodeInfo{
	int memoryLimitSize; // 内存限制大小
	int cpuLimit; // 处理器限制 100 分比
	PhysicNodeInfo()
	{
		memoryLimitSize = cpuLimit = 0;
	}
};

struct LogicNodeInfo{
	int nodeType; // 节点类型
	unsigned short port; // 端口
	enum{
		LOGIN_SERVER = 1 << 0,
		GATEWAY = 1 << 1,
		GLOAB_SERVER = 1 << 2,
		MANAGER_SERVER = 1 << 3,
		MAP_SERVER = 1 << 4,
		RECORD_SERVER = 1 << 5,
		RECORD_CENTER = 1 << 6,
	};
	// 屏蔽列表
	LogicNodeInfo()
	{
		nodeType = 0;
		port = 0;
	}
};

struct NodeInfo{
	char ip[32];
	unsigned short port;
};
struct UserData{
	char mapName[32];
};

struct RoleData{
	
};
#pragma pack()