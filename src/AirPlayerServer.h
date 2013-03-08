#ifndef __AIRPLAYERSERVER_H__
#define __AIRPLAYERSERVER_H__

#include "Server.h"

/*
 * 服务器类，默认的监听地址为localhost:8188
 * 单实例类
 */
class AirPlayerServer : public Server
{
    public:
    	//返回值为AirPlayerServer的实例
        static AirPlayerServer* getInstance(int portNum = 8188);
        
        ~AirPlayerServer();

        //获取共享的媒体目录
        const std::string getContent_dir();

    private:
    	//构造函数声明为私有的，通过getInstance函数来示例化
    	AirPlayerServer(int portNum = 8188, const std::string& content_dir = ".");
        
        //监听sockfd端口的请求并对请求进行处理
        void serve(int sockfd);
        
        //解析客户端的请求，处理并以字符串形式返回处理结果
        const std::string parse_and_process(char* request);
        
        //AirPlayerServer的实例
        static AirPlayerServer* instance;

        //设置共享媒体目录
        bool setContent_dir(const std::string& path);

        //共享媒体目录
        std::string content_dir;
};

#endif
