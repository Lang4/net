#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "mythread.h"
#include "mylist.h"
#include <stdarg.h>
#include <map>
#include <string>
class Logger:public  thread::Thread{
public:
	Logger(){
		writeStrategy = 0;
	}
	void run();
	void put(const std::string &info);
	void flush();
	mynet::MyList<std::string> infos;
	void init(const std::string fileName)
	{
		this->logName = fileName;
	}
	~Logger()
	{
		//final();
	}
	void print(const char *info,...);
private:	
	std::string logName; // 日志名字
	std::string makeFileName();// 创建文件 依据当前时间
	unsigned long getFileSize(const char *fileName);
	enum{
		NEED_CACHE = 1 << 0, // 需要缓存
		NEED_TIME = 1 << 1, // 需要记录时间
		NEED_CONSOLE = 1 << 2, // 需要写向控制台
		NEED_FILE = 1 << 3, // 需要写向文件
		NEED_FILE_SIZE = 1 << 4, // 文件的大小
	};
	int writeStrategy;
	bool checkFileExsit(const std::string &name);
};

class Loggers:public thread::Thread{
public:
	static Loggers & getMe()
	{
		static Loggers me;
		return me;
	}
	Loggers()
	{
		start();
	}
	Logger* get(const std::string &name)
	{
		LOGGERS_ITER iter = loggers.find(name);
		if (iter != loggers.end())
		{
			return iter->second;
		}
		else
		{
			loggers[name] = new Logger();
		}
		iter = loggers.find(name);
		if (iter != loggers.end())
		{
			return iter->second;
		}
		return NULL;
	}

	void run()
	{
		while(isAlive())
		{
			this->sleep(10); // 10s写一次
			for (LOGGERS_ITER iter = loggers.begin(); iter != loggers.end();++iter)
			{
				iter->second->flush();
			}
		}
	}
	std::map<std::string,Logger*> loggers;
	typedef std::map<std::string,Logger*>::iterator LOGGERS_ITER;
};

#define NET_LOG(info,...) do{\
char buffer[1024] = {'\0'};\
sprintf(buffer,"%s %s",__FUNCTION__,info);\
Loggers::getMe().get("net")->print(buffer,__VA_ARGS__);\
}while(false)