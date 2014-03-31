#pragma once
#include "Info.h"
#include "network.h"
#include "MyTick.h"
#include "MyServer.h"
/**
 * ����������ķ����߼�
 */

class ManagerLogic:public Logic
{
public:
	REMOTE_CLASS(ManagerLogic);
	/**
	 * �����ڵ�ע�ắ��(����Ψһ����,����,����);
	 */
	REMOTE_FUNCTION_1(reqLogin,const PhysicNodeInfo &);
	
	REMOTE_FUNCTION_2(reqLogin1,const PhysicNodeInfo &,int );
	REMOTE_FUNCTION_3(reqLogin2,const PhysicNodeInfo &,int ,int );
	REMOTE_FUNCTION_4(reqLogin3,const PhysicNodeInfo &,int ,int ,int );
	REMOTE_FUNCTION_5(reqLogin4,const PhysicNodeInfo &,int ,int ,int ,int );
	
};

class ManagerClient:public Logic
{
public:
	REMOTE_CLASS(ManagerClient);
	/**
	 * ����ڵ��֪��ǰ�ڵ������ķ�������
	 */
	REMOTE_FUNCTION_1(retStart,const LogicNodeInfo &); 
};

class ManagerTick:public MyTick{
public:
	ManagerTick():MyTick(1000){}
	/**
	 * 5s��ʱ�豸
	 */
	void tick()
	{
		// �����ڵ���Ϣ ���ڵ���Ϣ�ﵽ����
		// �㲥�ڵ���Ϣ �����ڵ����ɽ����ڵ�����
		printf("ManagerTick::tick\n");
	}
};