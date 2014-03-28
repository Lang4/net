#pragma once
#include "mynet.h"

class MyPool:public mynet::EventPool{
public:
	static MyPool& getMe()
	{
		static MyPool pool;
		return pool;
	}
	MyPool()
	{
		//init();
	}
};

#define thePool MyPool::getMe()