#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <string>
/*
 * 虚基类,派生类必须实现execute方法和toString方法
 */

class Command
{
    public:
    	Command();
    	virtual ~Command();
        virtual void execute(std::string& res) = 0;   
        virtual std::string toString() = 0;
        
    protected:
    //存放Command的处理结果
    	//std::string* res;

};

#endif
