#ifndef __COMMANDFACTORY_H__
#define __COMMANDFACTORY_H__
#include "Command.h"

/*
 * Command工厂，通过静态方法创建实例
 */
class CommandFactory
{
	public:
		//注意获取实例者有释放内存的责任
    	static Command* create(std::string type, const char* request);
};
#endif
