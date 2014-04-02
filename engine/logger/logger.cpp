#include "logger.h"
#include "windows.h"
#include "MyTime.h"
#include <sstream>
#include "io.h"
void set_console_color(unsigned short color_index)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_index);
}

void Logger::run()
{
	while(isAlive())
	{
		this->sleep(10); // 10s–¥“ª¥Œ
		flush();
	}
}
void Logger::flush()
{
	std::string temp;
	while(infos.readOnly(temp))
	{
		std::string fileName = makeFileName();
		FILE *hp = fopen(fileName.c_str(),"a+");
		if (hp)
		{
			std::stringstream ss;
			ss << temp <<"\n";
			fputs(ss.str().c_str(),hp);
			fclose(hp);
		}
		infos.pop();
	}
}
void Logger::print(const char *info,...)
{
	va_list ap;                                         
	va_start(ap, info); 
	char buffer[1024]={'\0'};
	vsprintf(buffer, info, ap );                            
	va_end(ap); 
/*
	set_console_color(FOREGROUND_GREEN);
	printf("%s ",MyTime::getNowTime().c_str());
	set_console_color(FOREGROUND_RED);
	printf(" %s \n",buffer);
	set_console_color(FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
*/
	std::stringstream ss;
	ss << MyTime::getNowTime() << " " << buffer;
	infos.write(ss.str());
	printf("%s\n",ss.str().c_str());
}
void Logger::put(const std::string &info)
{
	infos.write(info);
}
std::string Logger::makeFileName()
{
	unsigned long nowTime = ((int)MyTime::getNowSecs() / 3600) * 3600;
	unsigned int index = 0;
	while (true)
	{
		std::stringstream fileName;
		fileName << logName <<MyTime::getNowTime(nowTime);
		fileName <<"_"<<index <<".log";
		if (!checkFileExsit(fileName.str()))
		{
			return fileName.str();
		}
		if (getFileSize(fileName.str().c_str()) < 100000)
		{
			return fileName.str();
		}
		index ++;
	}
	return "CommLOG";
}
unsigned long Logger::getFileSize(const char *fileName)
{
	HANDLE hFile;
	__int64 i64Size;
	LARGE_INTEGER bigSize;
	hFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL,
					OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile==INVALID_HANDLE_VALUE) {
	return 0;
	}
	GetFileSizeEx(hFile, &bigSize);
	CloseHandle(hFile);
	i64Size = bigSize.QuadPart;
	return i64Size;
}

bool Logger::checkFileExsit(const std::string &name)
{
	if( (_access(name.c_str(), 0 )) != -1 ) return true;
	return false;
}