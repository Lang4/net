#pragma once
/**
 * 群组关注的逻辑部分 包括 角色 场景 可以由动态库的形式组织对逻辑的动态扩展
 * 提供创建 删除 消息处理
 */
class Role{
public:
	int gateID; // 选取当前的gateID
	// 可以有实现对象来处理具体的逻辑

};