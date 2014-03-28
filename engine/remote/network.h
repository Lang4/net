#pragma once

#include "remote.h"
#include <sstream>
namespace net{
	class Connection{
	public:
		virtual void send(void *cmd,unsigned int len) = 0;
		virtual ~Connection(){}
	};
	template<typename LOGIC>
	class Task:public Connection{
	public:
		LOGIC *logic;
		Task(){logic = NULL;}
		Task(LOGIC* logic):logic(logic){}
		void send(void *cmd,unsigned int len)
		{
			if (logic) logic->sendCmd(cmd,len);
		}
		void parse(void *cmd,unsigned int len)
		{
			remote::call(this,(char*)cmd,len);	
		}
	};
	template<typename T1>
	void call(Connection *conn,const std::string & name,T1 t1)
	{
		std::string content = remote::build<int>(name.c_str(),t1);
		if (content.size()) conn->send((void*) &content[0],content.size());
	}
	template<typename T1,typename T2>
	void call(Connection *conn,const std::string & name,T1 t1,T2 t2)
	{
		std::string content = remote::build<int>(name.c_str(),t1,t2);	
		if (content.size()) conn->send((void*) &content[0],content.size());
	}
	#define BIND_FUNCTION(func)\
		remote::bind(#func,func);
};


/*
	class FL_Task:public ServerTask:net::Task<FL_Task>
	{
	public:
		FL_Task():Task<FL_Task>(this){}

		void sayHello(int x,int y);
	}
	BIND(FL_Task::sayHello);

	call(conn,"FL_Task::sayHello",1,100);
 */


#define REMOTE_FUNCTION_1(NAME,ARG1)  static void R_##NAME(net::Connection *conn,ARG1 arg1){\
	net::call(conn,N_##NAME(),arg1);} static std::string N_##NAME(){std::stringstream ss; ss <<C_NAME()<<":"<<#NAME;return ss.str();}void NAME(ARG1)


#define BIND_NET_FUNCTION(CLASS,FUNC) remote::bind(CLASS::N_##FUNC().c_str(),&CLASS::FUNC); 


#define REMOTE_CLASS(NAME); static const char * C_NAME(){return #NAME;}

#define BEGIN_REMOTE_LOGIC(NAME) class NAME{ public:static const char * C_NAME(){return #NAME;}

#define END_REMOTE_LOGIC };

#define REMOTE_FUNCTION(NAME)  static std::string N_##NAME(){std::stringstream ss; ss <<C_NAME()<<":"<<#NAME;return ss.str();}

#define FUNCTION(NAME) static std::string N_##NAME(){std::stringstream ss; ss <<C_NAME()<<":"<<#NAME;return ss.str();} void FUNC
