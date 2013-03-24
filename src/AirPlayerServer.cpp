#include "AirPlayerServer.h"
#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include <dirent.h>
#include <string>
#include <cstdlib>
#include "CommandFactory.h"
#include <unistd.h>

using namespace std;

AirPlayerServer* AirPlayerServer::instance = NULL;

/*
 * 构造函数
 */
AirPlayerServer::AirPlayerServer(int portNum, const std::string& content_dir) : 
    Server(portNum), content_dir(content_dir)
{
    pthread_mutex_init(&mutex, NULL);                                                                    
}

AirPlayerServer::~AirPlayerServer()
{
    convertingMediaSet.clear();
    pthread_mutex_destroy(&mutex);
}

AirPlayerServer* AirPlayerServer::getInstance()
{
    if(instance == NULL)
    {
        instance = new AirPlayerServer();
    }
    return instance;
}

/*
 * 解析客户端的请求，处理并以字符串形式返回处理结果
 */
void AirPlayerServer::parse_and_process(char* request, int clfd, string& reply)
{
    char type[20];
    //读取命令的类型，如ls / play
    sscanf(request, "%s", type);
    
    //用工厂方法根据type来创建一个相应Command类
    Command* command = CommandFactory::create(type, request + strlen(type) + 1, clfd);    
    
    //res，临时储存execute的返回结果
    string res;
    
    //此处利用多态性执行对应子类的execute方法
    if(command != NULL)
    {
        command->execute(res);
    }
    
    //根据res生成用于发送给客户端的消息，储存于reply中
    if(res.empty())
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
    
    reply.append("\n");
}

void* AirPlayerServer::pthread_serve(void* arg){
    int clfd = *(int*)arg;
    char recv_buf[BUFSIZE];
    
    memset(recv_buf, 0, sizeof(recv_buf));
    
    //期望在一次recv操作中获取完客户端的请求
    int len = recv(clfd, recv_buf, BUFSIZE, 0);
    
    //如果客户端关闭了套接字
    if(len <= 0)
    {
        close(clfd);
        return (void*)-1;
        //exit(0);
    }

    //解析客户端的请求，处理并以字符串形式返回处理结果
    string reply;
    parse_and_process(recv_buf, clfd, reply);
    /*
     * 因为预期中的reply不会过长
     * 期望在一次send操作中完成传输
     */
     if(reply != string("OK\n@@@@@@\n")){
        send(clfd, reply.c_str(), reply.size(), 0);

        //关闭套接字
        close(clfd);
    }

    return 0;
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

        pthread_t tid;
        int arg = clfd;
        int err = pthread_create(&tid, NULL, pthread_serve, (void*)&arg);
        if(err != 0){

        }
        /*
         * fork一个子进程处理请求
         * 父进程继续监听
         */
        // int pid;
        // if((pid = fork()) < 0) 
        // {
        //     logInf(ERROR,"fork error");
        // }
        // else 
        // {
        //     if(pid == 0) 
        //     {   
        //         /* child */
        //         (void)close(listenfd);
                
        //         char recv_buf[BUFSIZE];
                
        //         memset(recv_buf, 0, sizeof(recv_buf));
                
        //         //期望在一次recv操作中获取完客户端的请求
        //         int len = recv(clfd, recv_buf, BUFSIZE, 0);
                
        //         //如果客户端关闭了套接字
        //         if(len <= 0)
        //         {
        //             close(clfd);
        //             exit(0);
        //         }

        //         //解析客户端的请求，处理并以字符串形式返回处理结果
        //         string reply;
        //         parse_and_process(recv_buf, clfd, reply);
        //         /*
        //          * 因为预期中的reply不会过长
        //          * 期望在一次send操作中完成传输
        //          */
        //          if(reply != string("OK\n@@@@@@")){
        //             send(clfd, reply.c_str(), reply.size(), 0);

        //             //关闭套接字
        //             close(clfd);
        //         }
                
        //         //正常结束子进程
        //         exit(0);
        //     } 
        //     else 
        //     {   
        //         /* parent */
        //         (void)close(clfd);
        //     }
        // }
    }
}

//获取共享的媒体目录
const std::string AirPlayerServer::getContent_dir()
{
    return content_dir;
}

//设置共享媒体目录
bool AirPlayerServer::setContent_dir(const std::string& path)
{
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
    return true;
}
#include <iostream>
bool AirPlayerServer::addConvertingMedia(const std::string& fp)
{
    bool r = false;
    pthread_mutex_lock(&mutex);
    for(set<string>::iterator it = convertingMediaSet.begin();
         it != convertingMediaSet.end(); it++){
        std::cout << *it << std::endl;
    }
    if(convertingMediaSet.find(fp) != convertingMediaSet.end()){
        r = false;
    }
    else{
        logInf(OK, string("converting video " + fp).c_str());
        convertingMediaSet.insert(fp);
        r = true;
    }
    pthread_mutex_unlock(&mutex);
    return r;
}

void AirPlayerServer::eraseConvertingMedia(const std::string& fp){
    pthread_mutex_lock(&mutex);
    convertingMediaSet.erase(fp);
    logInf(OK, string("finished converting video " + fp).c_str());
    pthread_mutex_unlock(&mutex);
}