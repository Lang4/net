/**
 * Ⱥ���ݴ����˵��
 * �������� �� �û����� 崻��������ڵ��崻��������������� �Լ� �����ȸ��� Ҳ��������������Ⱥ�� ��ɶԷ������ĸ���
 **/
/**
 * Ⱥ���߼��Ŀ���
 * Ⱥ���߼������������ ����ͬʱʹ��windows , linux �¿�������, ����˶�jygame���ع�����չ ���ڿ���windows ������
 * �����ڲ��ı�flash �˵������ �Է����������ع� ��֧�ŵ�����PK ͬʱ�Խ�ɫ���ݽ��ж�̬Ǩ��
 */
#pragma once
#include "MyServer.h"
#include "Connection.h"
class Server:public MyServer<LogicConnection>{
public:
	void beInit();
	void beFinal();
};
extern Server server;
