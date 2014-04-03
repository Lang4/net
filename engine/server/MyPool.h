#pragma once
#include "mynet.h"
#include "pointer.h"
#include "MyTick.h"
#include "MyConnection.h"
#include <list>
#include "mythread.h"
class MyPool:public mynet::EventPool,public MyTick{
public:
	static MyPool& getMe()
	{
		static MyPool pool;
		return pool;
	}
	thread::Mutex lock;
	MyPool()
	{
		theTick.addTick(this);
	}
	void add(mynet::Connection *conn)
	{
		thread::Mutex_scope_lock scope(lock);
		for (POINTERS_ITER iter = add_pointers.begin(); iter != add_pointers.end();++iter)
		{
			if (conn == iter->pointer()) return;
		}
		add_pointers.push_back(Pointer<mynet::Connection>(conn));
	}
	void remove(mynet::Connection *conn)
	{
		for (POINTERS_ITER iter = pointers.begin(); iter != pointers.end();++iter)
		{
			if (conn == iter->pointer())
			{
				iter->setState(1);
				return;
			}
		}
	}
	std::list<Pointer<mynet::Connection> > add_pointers;
	std::list<Pointer<mynet::Connection> > pointers;
	typedef std::list<Pointer<mynet::Connection> >::iterator POINTERS_ITER;
	void doAdd()
	{
		thread::Mutex_scope_lock scope(lock);
		pointers.insert(pointers.end(),add_pointers.begin(),add_pointers.end());
		add_pointers.clear();
	}
	void tick()
	{
		doAdd();
		for (POINTERS_ITER iter = pointers.begin(); iter != pointers.end();)
		{
			if (iter->checkState(1))
			{
				iter = pointers.erase(iter);
				break;
			}
			MyConnection *tick = (MyConnection*)iter->pointer();
		    if (tick && tick->checkActive())
			{
				tick->tick();
			}
			++iter;
		}
	}
	~MyPool()
	{
		theTick.removeTick(this);
	}
};

#define thePool MyPool::getMe()