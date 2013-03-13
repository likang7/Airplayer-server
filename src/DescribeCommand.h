#ifndef __DESCRIBECOMMAND__H
#define __DESCRIBECOMMAND__H

#include "Command.h"

/*
 * DescribeCommand继承于Command，需要实现Command中的execute方法。
 * 执行成功，返回视频文件的相关属性
 * 执行失败，返回一个NULL
 */
class DescribeCommand : public Command
{
    public:
        DescribeCommand(const char* _request);
        /*
         *execute后返回一个视频文件的相关属性
         *如果无法获得属性，返回NULL
         */
        virtual const char* execute();
        virtual std::string toString();
    private:
       int getMediainfo(const std::string fp);
       int getFileSize(const std::string fp);
    	//request的格式为describe [pathname] 
        const char* request;
        std::string duration;
        std::string resolution;
        std::string filesize;
};
#endif
