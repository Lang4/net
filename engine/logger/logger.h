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
	std::string logName; // ��־����
	std::string makeFileName();// �����ļ� ���ݵ�ǰʱ��
	unsigned long getFileSize(const char *fileName);
	enum{
		NEED_CACHE = 1 << 0, // ��Ҫ����
		NEED_TIME = 1 << 1, // ��Ҫ��¼ʱ��
		NEED_CONSOLE = 1 << 2, // ��Ҫд�����̨
		NEED_FILE = 1 << 3, // ��Ҫд���ļ�
		NEED_FILE_SIZE = 1 << 4, // �ļ��Ĵ�С
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
			this->sleep(10); // 10sдһ��
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