#ifndef __MQUALITY_H__
#define __MQUALITY_H__

#include"Quality.h"

using namespace std;

//中等质量策略类
class Mquality:public Quality
{
	public:
		Mquality(std::string);
		~Mquality();
		virtual struct Property * set(struct Property *);


};


#endif
