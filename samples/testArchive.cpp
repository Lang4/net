#pragma once
#include "archive.h"
class Object{
public:
	int v;
	char b;
	short c;
	long long d;
	std::vector<char> e;
};
class Object1{
public:
	int v;
	std::vector<Object> s;
};
IMP_SERIALIZE_OBJECT(Object)
{
	BD(v,1);
	BD(b,2);
	BD(c,3);
	BD(d,4);
	BD(e,5);
}
IMP_SERIALIZE_OBJECT(Object1)
{
	BD(v,1);
	BD(s,2);
}

void test()
{
	Object o1;
	o1.v = 100;
	o1.b = 'c';
	o1.c = 10000;
	o1.d = 100000;
	o1.e.push_back('d');
	o1.e.push_back('e');

	Object1 o2;
	o2.v = 1000000;
	o2.s.push_back(o1);
	BYTE_ARRAY *out;
	Stream ss(o2,&out);
	
	Object1 o3;
	Stream ss2(o3,out);

	delete out;
}