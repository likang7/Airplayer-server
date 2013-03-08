#ifndef __HQUALITY_H__
#define __HQUALITY_H__

#include"Quality.h"

using namespace std;

//高质量策略类
class Hquality:public Quality
{
	public:
		Hquality(std::string);
		~Hquality();
		virtual struct Property * set(struct Property *);
		


};


#endif
