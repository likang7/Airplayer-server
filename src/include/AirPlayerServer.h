#ifndef __AIRPLAYERSERVER_H__
#define __AIRPLAYERSERVER_H__

#include "Server.h"
#include <set>
#include <pthread.h>
/*
 * 服务器类，默认的监听地址为localhost:8188
 * 单实例类
 */
class AirPlayerServer : public Server
{
    public:
    	//返回值为AirPlayerServer的实例
        static AirPlayerServer* getInstance();
        
        ~AirPlayerServer();

        //获取共享的媒体目录
        const std::string getContent_dir();

        //设置共享媒体目录
        bool setContent_dir(const std::string& path);

        bool addConvertingMedia(const std::string& fp);

        void eraseConvertingMedia(const std::string& fp);

    private:
    	//构造函数声明为私有的，通过getInstance函数来示例化
    	AirPlayerServer(int portNum = 8188, const std::string& content_dir = ".");
        
        //监听sockfd端口的请求并对请求进行处理
        void serve(int sockfd);

        //
        static void* pthread_serve(void* arg);
        
        //解析客户端的请求，处理并以字符串形式返回处理结果
        static void parse_and_process(char* request, int clfd, std::string& reply);
        
        //AirPlayerServer的实例
        static AirPlayerServer* instance;

        //共享媒体目录
        std::string content_dir;

        //
        std::set<std::string> convertingMediaSet;

        pthread_mutex_t mutex;
};

#endif
