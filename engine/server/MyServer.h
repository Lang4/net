#pragma once
#include "mynet.h"
#include "MyPool.h"
#include "MyConnection.h"
template<typename CONNECTION>
class MyServer:public mynet::stGetPackage{
public:
	virtual void beInit() {};
	virtual void beFinal(){};
	void init(const char*ip,unsigned short port)
	{
		mynet::Server server(ip,port);
		for (int i =0;i < 10;i++)
		thePool.bindEvent(&server,mynet::ACCEPT_EVT);
		stop = false;
	}
	void final()
	{
		stop = true;
	}
	MyServer()
	{
		stop = false;
		nodeType = 0;
	}
	bool stop;
	void go()
	{
		beInit();
		while (!stop)
		{
			mynet::EventBase *evt = thePool.pullEvent();
			if (evt)
			{
				if (evt->isAccept()) 
				{
					CONNECTION *conn =  new CONNECTION();
					conn->setHandle(evt->getPeerHandle());
					thePool.bindEvent(conn,mynet::IN_EVT|mynet::OUT_EVT);
					conn->beInit();
		//			pool.bindEvent(&server,mynet::ACCEPT_EVT);
					continue;
				}
				if (evt->isErr())
				{
					CONNECTION *conn = (CONNECTION*) evt->target;
					if (conn)
					{
						conn->destroy();
						if (conn->beFinal())
							delete conn;
					}
					printf("删除网络连接%p\n",evt->target);
					continue;
				}
				if (evt->isOut()) 
				{
					mynet::Connection *conn = (mynet::Connection*) evt->target;
					if (conn)
					{
						conn->doSend(evt);
					}
					//printf("out");
				}
				if (evt->isIn()) 
				{
					mynet::Connection *conn = (mynet::Connection*) evt->target;
					if (conn)
					{
						conn->doRead(evt,this);
					}
					//printf("in");
				}
				
			}
		}
		beFinal();
	}
	/**
	 * 文件需要请求两次才能请求到实际数据
	 */
	virtual void doGetCommand(mynet::Target *target,void *cmd,unsigned int len)
	{
		MyConnection *conn = static_cast<MyConnection*>(target);
		if (conn)
		{
			conn->doCmd(cmd,len);
		}
	}
	unsigned int nodeType;
};



class Logics:public LogicCenter{
public:
	virtual void dispatch(MyConnection *connection,const std::string &className,char *cmd,unsigned int len);
	Logics()
	{
	
	}

	static Logics& getMe()
	{
		static Logics me;
		return me;
	}

	void setup()
	{
		thePool.init();
		theTick.start();
		init();
	}

	void init();
};

#define theLogics Logics::getMe()