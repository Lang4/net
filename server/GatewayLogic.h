#pragma once
#include <string>
#include "network.h"
class GatewayLogic{
public:
	REMOTE_CLASS(GatewayLogic);
	int logicID; // �߼��ڵ���
	int mapID; // ��ͼ���������
	/**
	 * ������
	 */
	bool filter(void *cmd,unsigned int len)
	{
		// ���1s������Ϣ��̫�� ��ص�����

		return true;
	}
	/**
	 * ��ʱ��Ϊ
	 */
	void tick()
	{
		// �� ns������Ϣǰ�� ��Ͽ�����
	}

	void doLogin(const std::string &name,const std::string& pwd)
	{
		// �����¼
		//records.get(logicID,name,"GatewayLogic::backData",1000,"GatewayLogic::backDataTimeout");
	}

	/**
	 * ����ص�����
	 */
	void backData(const UserData &data)
	{
		// ��֤����
		// ȷ����������
		//maps.get(logicID,data.mapName,"GatewayLogic::intoScene");
	}
	/**
	 * ����ص����ݳ�ʱ
	 */
	void backDataTimeout()
	{
		//�Ͽ�����
	}

	void intoScene()
	{
		//if (scene)
		{
			//remote::call(scene->getConnection(),"Scene::intoScene",data); ��¼�������ڵ�
		}
	}
	void sceneInValid()
	{
		// ����ʧЧʱ ����ѡȡ
		// ���ߵ�¼����
	}
	REMOTE_FUNCTION_1(test,const std::string &)
	{
		printf("test\n");
	}
};