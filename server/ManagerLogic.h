#pragma once
#include "Info.h"
#include "network.h"
#include "MyTick.h"
/**
 * ����������ķ����߼�
 */

class ManagerLogic{
public:
	REMOTE_CLASS(ManagerLogic);
	/**
	 * �����ڵ�ע�ắ��(����Ψһ����,����,����);
	 */
	REMOTE_FUNCTION_1(reqLogin,const PhysicNodeInfo &);

};

class ManagerClient
{
public:
	REMOTE_CLASS(ManagerClient);
	/**
	 * ����ڵ��֪��ǰ�ڵ������ķ�������
	 */
	REMOTE_FUNCTION_1(retStart,const LogicNodeInfo &); 

	REMOTE_FUNCTION(otherFunc);
	void otherFunc(int a); // ������ʽ2
 
	FUNCTION(otherFunc1) (int b); // ������ʽ3
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
		printf("1\n");
	}
};