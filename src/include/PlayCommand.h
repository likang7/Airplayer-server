#ifndef __PLAYCOMMAND__H
#define __PLAYCOMMAND__H

#include "Command.h"
#include "Property.h"
#include "MediaInfo.h"
#include <string>

/*
 * playCommand继承于Command，需要实现Command中的execute方法。
 * 执行成功，返回一个RTSP地址
 * 执行失败，返回一个NULL
 */
class PlayCommand : public Command
{
    public:
        PlayCommand(const char* _request);
        /*
         *execute后返回结果为一个可访问的rtsp地址。
         *如果无法获得rtsp地址，返回NULL
         */
        virtual void execute(std::string& res);
        virtual std::string toString();
    private:
    	//request的格式为filepath, 转码请求(如quality:xxx, fps:xxx)
        const char* request;
        int parse(char* str, Property* p);
        bool checkRewrite(const std::string& fp);
        MediaInfo media;
};
#endif
