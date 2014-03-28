#pragma once

class RecordLogic{
public:
	/**
	 * 获取数据
	 */
	void reqData(const std::string &whereStr);
	/**
	 * 获取数据所在的节点
	 */
	void reqNodeByData(const std::string &whereStr);
	
	/**
	 * 保存数据
	 */
	void saveData(const RoleData &data);

	/**
	 * 保存数据到具体的节点
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
		// 检索到引索好 回调相应的函数 异步操作
	}
};

/**
 * 会有N个RecordClient 被记录
 */