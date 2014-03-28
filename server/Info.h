#pragma once

#pragma pack(1)

struct PhysicNodeInfo{
	int memoryLimitSize; // �ڴ����ƴ�С
	int cpuLimit; // ���������� 100 �ֱ�
	PhysicNodeInfo()
	{
		memoryLimitSize = cpuLimit = 0;
	}
};

struct LogicNodeInfo{
	int nodeType; // �ڵ�����
	unsigned short port; // �˿�
	enum{
		LOGIN_SERVER = 1 << 0,
		GATEWAY = 1 << 1,
		GLOAB_SERVER = 1 << 2,
		MANAGER_SERVER = 1 << 3,
		MAP_SERVER = 1 << 4,
		RECORD_SERVER = 1 << 5,
		RECORD_CENTER = 1 << 6,
	};
	// �����б�
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