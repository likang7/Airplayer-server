#include "AirPlayerServer.h"
#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include <dirent.h>
#include <string>
#include <cstdlib>
#include "CommandFactory.h"

using namespace std;

AirPlayerServer* AirPlayerServer::instance = NULL;

/*
 * 构造函数
 */
AirPlayerServer::AirPlayerServer(int portNum, const std::string& content_dir) : 
	Server(portNum), content_dir(content_dir)
{
                                                                                    
}

AirPlayerServer::~AirPlayerServer()
{

}

AirPlayerServer* AirPlayerServer::getInstance(int portNum)
{
	if(instance == NULL)
	{
		instance = new AirPlayerServer(portNum);
	}
	return instance;
}

/*
 * 解析客户端的请求，处理并以字符串形式返回处理结果
 */
const string AirPlayerServer::parse_and_process(char* request)
{
	char type[20];
	//读取命令的类型，如ls / play
	sscanf(request, "%s", type);
	
	//用工厂方法根据type来创建一个相应Command类
	Command* command = CommandFactory::create(type, request + strlen(type) + 1);	
	
	//res，临时储存execute的返回结果
	const char* res = NULL;
	
	//此处利用多态性执行对应子类的execute方法
	if(command != NULL)
	{
		res = command->execute();
	}
	
	//根据res生成用于发送给客户端的消息，储存于reply中
	string reply;
	if(res == NULL)
	{
		if(command != NULL)
		{
			reply = string("ERROR cannot ") + command->toString();
		}
		else
		{
			/*
			 * 如果没有对应的Command类，返回出错信息给用户
			 */
			reply = (string("ERROR Unknown command : ") + type).c_str();
		}
		logInf(WARNING, reply.c_str());
	}
	else
		reply = string("OK\n") + res;
	
	//释放内存
	delete command;
	
	return reply + "\n";
}

/*
 *监听sockfd端口的请求并对请求进行处理
 */
void AirPlayerServer::serve(int sockfd)
{ 
	//循环监听请求
    while(true)
	{
    	//clfd为accept函数返回的套接字描述符
    	int clfd = -1;
    	
        clfd = accept(sockfd, NULL, NULL);
        if(clfd < 0)
		{
            logInf(ERROR, "accept error");
        }
        
        /*
		 * fork一个子进程处理请求
         * 父进程继续监听
		 */
        int pid;
        if((pid = fork()) < 0) 
		{
			logInf(ERROR,"fork error");
		}
		else 
		{
			if(pid == 0) 
			{ 	
				/* child */
				(void)close(listenfd);
				
				char recv_buf[BUFSIZE];
				
				memset(recv_buf, 0, sizeof(recv_buf));
				
				//期望在一次recv操作中获取完客户端的请求
				int len = recv(clfd, recv_buf, BUFSIZE, 0);
				
				//如果客户端关闭了套接字
				if(len <= 0)
				{
					close(clfd);
					exit(0);
				}

				//解析客户端的请求，处理并以字符串形式返回处理结果
				const string reply = parse_and_process(recv_buf);
				
				/*
				 * 因为预期中的reply不会过长
				 * 期望在一次send操作中完成传输
				 */
				send(clfd, reply.c_str(), reply.size(), 0);

				//关闭套接字
				close(clfd);
				
				//正常结束子进程
				exit(0);
			} 
			else 
			{ 	
				/* parent */
				(void)close(clfd);
			}
		}
    }
}

//获取共享的媒体目录
const std::string AirPlayerServer::getContent_dir(){
	return content_dir;
}

//设置共享媒体目录
bool AirPlayerServer::setContent_dir(const std::string& path){
	//检验路径是否存在且能打开，并且是否为一个目录
    struct stat _stat;
    DIR *dp;
    if(lstat(path.c_str(), &_stat) < 0 || !S_ISDIR(_stat.st_mode) 
    	|| ((dp = opendir(path.c_str())) == NULL))
    {
    	logInf(ERROR, "cannot access the content_dir");
    	return false;
    }
    this->content_dir = path;
}
