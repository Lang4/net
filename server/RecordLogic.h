#pragma once

class RecordLogic{
public:
	/**
	 * ��ȡ����
	 */
	void reqData(const std::string &whereStr);
	/**
	 * ��ȡ�������ڵĽڵ�
	 */
	void reqNodeByData(const std::string &whereStr);
	
	/**
	 * ��������
	 */
	void saveData(const RoleData &data);

	/**
	 * �������ݵ�����Ľڵ�
	 */
	void saveDataToNode(const RoleData &data);
};

class RecordClient{
public:
	void retIPByData(NodeInfo &info)
	{
		// remote::call(getConnectionByInfo(info),"RecordLogic::reqData",whereInfo);
	}

	void retData(RoleData &data)
	{
		// ������������ �ص���Ӧ�ĺ��� �첽����
	}
};

/**
 * ����N��RecordClient ����¼
 */