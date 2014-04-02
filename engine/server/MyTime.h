#pragma once
#include <string>
class MyTime{
public:
	static double getNowSecs();
	static double getNowMSecs();
	static std::string getNowTime();
	static std::string getNowTime(unsigned long nowTime);
};