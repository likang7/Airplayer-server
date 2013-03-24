#include "AirPlayerServer.h"
#include "mediaServer/DynamicRTSPServer.h"
#include <BasicUsageEnvironment.hh>
#include <stdio.h>
#include <iostream>
#include <sys/stat.h>
#include <dirent.h>
#include <cstdlib>
#include <cstring>
#include <pthread.h>
#include <string>
using std::cout;
using std::endl;
using std::string;


#define linux 1

//airplayerServer的端口号
int portNum = 8188;

int rtspServerPortNum = 8554;

//ffmpeg的路径
char* pathFFMPEG = (char*)"/home/lk/ffmpeg";

//content path
char* cp = (char*)".";

/*
 * 开启ffserver服务器
 */
 bool openffserver(const char* path = "./ffserver")
{
    pid_t pid;
    /*
     * vfork确保子进程先执行exec操作
     */
    if((pid = vfork()) < 0)
    {
        cout << "fork error" << endl;
        exit(1);
    }

    bool success = true;
    if(pid == 0)//child
    {
        if(execl(path, "ffserver", \
                    "-f", "ffserver.conf", (char*)0) < 0)
        {
            cout << "ffserver not found" << endl;
            success = false;
            exit(1);
        }
        exit(0);
    }
    else
    //主进程等待1s，然后返回
    {
        sleep(1);
        cout << "**************************************" << "\n"
             << "**************************************" << endl;
        return success;
    }
}

void* createMediaServer(void* arg){
    TaskScheduler* scheduler = BasicTaskScheduler::createNew();
    UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);
    UserAuthenticationDatabase* authDB = NULL;
    // Create the RTSP server with the port number:
    RTSPServer* rtspServer;
    rtspServer = DynamicRTSPServer::createNew(*env,
    rtspServerPortNum, authDB);
    if (rtspServer == NULL) {
        *env << "Failed to create RTSP server: " <<
        env->getResultMsg() << "\n";
        exit(1);
    }
    else{
        *env << "RTSP server created." << "\n";
    }
    env->taskScheduler().doEventLoop(); // does not return
    return((void*)0);
}
/*
 * 开启mediaserver
 */
bool openmediaserver(){
    pthread_t tid;
    
    int err = pthread_create(&tid, NULL, createMediaServer, NULL);

    if(err != 0){
        cout << "CANNOT CREATE MEDIASERVER" << endl;
        exit(1);
    }

    return true;
}
/*
 * 输出帮助信息
 */
void printHelp()
{
    cout << "=====================  help  ===================" << "\n"
         << "usage: ./apserver \"portNumber\" \"pathname\"" << "\n"
         << "portNumber : the port number of airPlayerServer" << "\n"
         << "pathname : the path of ffmpeg" << "\n"
         << "default : ./apserver 8188 ./ffmpeg" << "\n"
         << "================================================" << endl;
}

/*
 * 初始化端口号以及ffmpeg路径
 */
void init(int argv, char* args[]){
    if(argv > 1)
    {
        //if the first argument is "help", print help information
        if(strcmp("--help", args[1]) == 0)
        {
            printHelp();
            exit(0);
        }
        else
        {
            cp = args[1];
        }
    }

    if(argv > 2){
        //设置端口号
        portNum = atoi(args[2]);
        if(portNum <= 1023)
        {
            cout << "ERROR: invalid portNum : " << args[1] << endl;
            exit(1);
        }
    }

    //设置ffmpeg的路径
    if(argv > 3)
    {
        pathFFMPEG = args[3];
    }
}

int main(int argv, char* args[])
{
    //初始化
    init(argv, args);
    
    bool status1, status2;
    //启动ffserver
    //status1 = openffserver(pathFFserver);
    status2 = openmediaserver();
    if(status2 == false){
        cout << "fail to create live555MediaServer" << endl;
        return -1;
    }
    //启动AirPlayerServer
    AirPlayerServer* server = AirPlayerServer::getInstance();
    server->setPort(portNum);
    server->setContent_dir(cp);
    server->run();

    return 0;
}
