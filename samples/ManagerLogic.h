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