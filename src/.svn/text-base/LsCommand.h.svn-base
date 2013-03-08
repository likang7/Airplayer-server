#ifndef __LSCOMMAND__H
#define __LSCOMMAND__H

#include "Command.h"

/*
 * LsCommand,Command的派生类
 * 接受一个目录的路径，以字符串形式返回该目录下的文件信息
 */
class LsCommand : public Command
{
    public:
        LsCommand(const char* _path);
        virtual ~LsCommand();
        /*
         *path为目录的路径，以字符串形式返回该目录下的文件信息
         *失败返回NULL
         */
        virtual const char* execute();
        virtual std::string toString();
    private:
        const char* path;
};
#endif
