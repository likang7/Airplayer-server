#ifndef __SERVER_H__
#define __SERVER_H__

#include <string>
#include <netinet/in.h>
#define BUFSIZE 8096

enum TYPE
{
	ERROR,
	WARNING,
	OK
};

/*
 * 服务器虚基类
*/
class Server
{
    public:
        Server(int portNum);
        
        virtual ~Server();
        
        //使进程成为守护进程
        void daemonize(const std::string cmd);
        
        //运行服务器
        virtual void run();
        
        //获取服务器监听的端口号
        int getPort() const;
        
        //设置服务器的监听端口号，要在run之前设置
        void setPort(int portNum);
        
        /*
 		 *获取本机的IPv4地址，返回值为ipv4地址或者主机名的字符串指针
 		 *失败返回NULL
 		 */
        const char* getLocalIP();
        
    protected:
    	//日志函数，记录服务器工作情况
        static void logInf(TYPE type, const std::string str);
        
        virtual void init();
        
        //服务方法，纯虚函数，由子类实现
        virtual void serve(int sockfd) = 0;
        
        //服务器名字
        std::string hostname;
        
        //服务器监听的套接字
        int listenfd;
        
        //服务器套接字地址
        struct sockaddr_in serv_addr;          
};

#endif
