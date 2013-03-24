#ifndef __HOMECOMMAND__H
#define __HOMECOMMAND__H

#include "Command.h"

/*
 * HomeCommand,Command的派生类
 * 以字符串形式返回媒体目录的路径
 */
class HomeCommand : public Command
{
    public:
        HomeCommand(const char* _path = "");
        virtual ~HomeCommand();
        /*
         *以字符串形式返回媒体目录的路径
         *失败返回NULL
         */
        virtual void execute(std::string& res);
        virtual std::string toString();
    private:
        const char* path;
};
#endif
