#include "MyServer.h"
#include "MyPool.h"
#include "network.h"
#include "MyClient.h"
#include "ManagerLogic.h"
/**
 * 每个连接的处理
 */
class LogicConnection:public MyConnection{
public:
	void beInit()
	{
		
	}
	~LogicConnection()
	{
		printf("我卸载啦\n");
	}
};
/**
 * 每个客户端
 */
class Client:public MyClient{
public:
	Client(const char *ip,unsigned short port):MyClient(ip,port){
		theTick.addTick(this);
		this->registerLogics(&theLogics);
	}
	void beInit()
	{
		
	}
	bool beFinal()
	{
		socket = -1;
		return true;
	}
};
/** 
 * 服务节点
 **/
class Server:public MyServer<LogicConnection>{
public:
	void beInit()
	{
		printf("连接她\n");
		
		PhysicNodeInfo info;
		ManagerLogic::R_reqLogin(URL<Client>::Get("127.0.0.1",1234),info);
	}
	void beFinal()
	{
	
	}
};
void logger();
int main()
{
	logger();
	theLogics.setup();
	
	Server server;
	server.init("127.0.0.1",1235);
	
	server.go();
}
#include <log4cxx/logger.h>
#include <log4cxx/PropertyConfigurator.h>

using namespace std;
using namespace log4cxx;
void logger()
{
	string trace = "fa";
    string Property = "./log.properties";
    log4cxx::PropertyConfigurator::configure(Property);
    LoggerPtr logger = Logger::getLogger(trace);
    logger->info("info, How to use?");
    logger->debug("debug, How to use?");
    logger->warn("warn, How to use?");
    logger->error("error, How to use?");

}