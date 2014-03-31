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
		std::string content = remote::build<std::string,int>(name,t1);
		if (content.size()) conn->send((void*) &content[0],content.size());
	}
	template<typename T1,typename T2>
	void call(Connection *conn,const std::string & name,T1 t1,T2 t2)
	{
		std::string content = remote::build<std::string,int>(name.c_str(),t1,t2);	
		if (content.size()) conn->send((void*) &content[0],content.size());
	}
	template<typename T1,typename T2,typename T3>
	void call(Connection *conn,const std::string & name,T1 t1,T2 t2,T3 t3)
	{
		std::string content = remote::build<std::string,int>(name.c_str(),t1,t2,t3);	
		if (content.size()) conn->send((void*) &content[0],content.size());
	}
	template<typename T1,typename T2,typename T3,typename T4>
	void call(Connection *conn,const std::string & name,T1 t1,T2 t2,T3 t3,T4 t4)
	{
		std::string content = remote::build<std::string,int>(name.c_str(),t1,t2,t3,t4);	
		if (content.size()) conn->send((void*) &content[0],content.size());
	}
	template<typename T1,typename T2,typename T3,typename T4,typename T5>
	void call(Connection *conn,const std::string & name,T1 t1,T2 t2,T3 t3,T4 t4,T5 t5)
	{
		std::string content = remote::build<std::string,int>(name.c_str(),t1,t2,t3,t4,t5);	
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


#define REMOTE_FUNCTION_2(NAME,ARG1,ARG2)  static void R_##NAME(net::Connection *conn,ARG1 arg1,ARG2 arg2){\
	net::call(conn,N_##NAME(),arg1,arg2);} static std::string N_##NAME(){std::stringstream ss; ss <<C_NAME()<<":"<<#NAME;return ss.str();}void NAME(ARG1,ARG2)


#define REMOTE_FUNCTION_3(NAME,ARG1,ARG2,ARG3)  static void R_##NAME(net::Connection *conn,ARG1 arg1,ARG2 arg2,ARG3 arg3){\
	net::call(conn,N_##NAME(),arg1,arg2,arg3);} static std::string N_##NAME(){std::stringstream ss; ss <<C_NAME()<<":"<<#NAME;return ss.str();}void NAME(ARG1,ARG2,ARG3)


#define REMOTE_FUNCTION_4(NAME,ARG1,ARG2,ARG3,ARG4)  static void R_##NAME(net::Connection *conn,ARG1 arg1,ARG2 arg2,ARG3 arg3,ARG4 arg4){\
	net::call(conn,N_##NAME(),arg1,arg2,arg3,arg4);} static std::string N_##NAME(){std::stringstream ss; ss <<C_NAME()<<":"<<#NAME;return ss.str();}void NAME(ARG1,ARG2,ARG3,ARG4)

#define REMOTE_FUNCTION_5(NAME,ARG1,ARG2,ARG3,ARG4,ARG5)  static void R_##NAME(net::Connection *conn,ARG1 arg1,ARG2 arg2,ARG3 arg3,ARG4 arg4,ARG5 arg5){\
	net::call(conn,N_##NAME(),arg1,arg2,arg3,arg4,arg5);} static std::string N_##NAME(){std::stringstream ss; ss <<C_NAME()<<":"<<#NAME;return ss.str();}void NAME(ARG1,ARG2,ARG3,ARG4,ARG5)

#define BIND_NET_FUNCTION(CLASS,FUNC) remote::bind(CLASS::N_##FUNC(),&CLASS::FUNC); 


#define REMOTE_CLASS(NAME); static const char * C_NAME(){return #NAME;} void NAME::__init__();

#define BEGIN_REMOTE_LOGIC(NAME) class NAME{ public:static const char * C_NAME(){return #NAME;}

#define END_REMOTE_LOGIC };

#define REMOTE_FUNCTION(NAME)  static std::string N_##NAME(){std::stringstream ss; ss <<C_NAME()<<":"<<#NAME;return ss.str();}

#define FUNCTION(NAME) static std::string N_##NAME(){std::stringstream ss; ss <<C_NAME()<<":"<<#NAME;return ss.str();} void FUNC

