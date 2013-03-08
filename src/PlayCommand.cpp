#include "PlayCommand.h"
#include "AirPlayerServer.h"
#include "ConfigFile.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

using namespace std;
//using std::string;
#define BUFFER_SIZE 255 
#define REQUEST_FIFO "/tmp/request_fifo" 
//#define DEBUG

int getline_from_fifo(int fd_read,char* str, int n)
{
    int index=0;
    char ch; 
    #ifdef DEBUG
    printf("fuck!");
    #endif
    while(index<(n-1))
    {
        if(read(fd_read,&ch,1)<=0)
        {
            #ifdef DEBUG
            printf("fuck <0\n");
            #endif
            sleep(1);
            return index;
        } 
        if(ch=='\n')
        {
            str[index++]='\0';
            return index;
        }
        str[index++]=ch;
        #ifdef DEBUG
        printf("%c",ch);
        #endif
    }
    str[index++]='\0';
    #ifdef DEBUG
    printf("fuck end\n");
    #endif
    return index;
}

//request的格式为filepath, 转码请求(如quality:xxx, fps:xxx)
PlayCommand::PlayCommand(const char* _request) :
    request(_request)
{

}

/*
 *execute后返回结果为一个可访问的rtsp地址。
 *如果无法获得rtsp地址，返回NULL
 */
const char* PlayCommand::execute()
{
    if(res != NULL)
        return res->c_str();

    /*
     *以下部分需要先解析request，提取出文件路径和编码请求
     *设置好ffserver的配置文件后返回rtsp地址
     */
    char* req = new char[200];
    char* temp;

    string confFile;
    ConfigFile *configFile;
    pid_t pid;
    char read_fifo_path[255];

    strcpy(req,request);
    temp = strchr(req,',');
    if(temp == NULL)
    {
    	return NULL;
    }

    temp++;
    //cout << temp <<endl;
    req = strtok(req,",");
	//cout << req <<endl;
	
    struct stat _stat;
    DIR *dp;
    if(lstat(req, &_stat) >= 0 && S_ISREG(_stat.st_mode))
    {
    	
    	string confFile = "./ffserver.conf";
		string str = string(temp); 
		string videoName = string(req);
		//cout << req << str <<endl;
    	configFile = new ConfigFile(confFile,videoName);
    	configFile->setVedioQuality(str);

        //若选择直接播放，返回live555MediaServer的rtsp地址
        if(configFile->getIsDirect()){
            res = new string();
            AirPlayerServer* server = AirPlayerServer::getInstance();
            *res = string("rtsp://") + server->getLocalIP() + ":8554/" + videoName;
            delete [] req;
            delete configFile;
            return res->c_str();
        }	

    	char command[BUFFER_SIZE];

    	pid = getpid();
    	confFile = "./ffserver.conf";
    	sprintf(command,"loadconf %s %u \n",confFile.c_str(),pid);
    	sprintf(read_fifo_path, "/tmp/return_state_fifo.%u", pid);

    	if(system("rm -f /tmp/return_state_fifo*") < 0){
            printf("cannot delete /tmp/return_state_fifo*");
            return NULL;
        } 
		mkfifo(read_fifo_path,0777);
    
    	#ifdef DEBUG
    	printf("str = %s\n",confFile.c_str()); 
    	printf("str = %s\n",command); 
    	#endif
    
    	if(access(read_fifo_path, F_OK) != 0)
    	{
        	printf("%s access faild\n",read_fifo_path);
        	return NULL;
    	}

    	if(access(REQUEST_FIFO, F_OK) != 0)
    	{
        	printf("request_fifo access faild\n");
        	return NULL;
    	}

    	if(!fifo_send(command,read_fifo_path))
    	{
		return NULL;
    	}
   
        memset(command, '*', strlen(command) );


        res = new string();
        AirPlayerServer* server = AirPlayerServer::getInstance();
        
        *res = string("rtsp://") + server->getLocalIP() + ":" + configFile->getRTSPServerPort() + "/"   \
            + "rtspVideo\n";
        sprintf(command,"convert %s http://127.0.0.1:%s/feed1.ffm %u \n",req,configFile->getHTTPServerPort().c_str(),pid);
        
        #ifdef DEBUG
        printf("str = %s\n",command);
        #endif
        if(!configFile->getIsDirect())
		{
        	if(!fifo_send(command,read_fifo_path))
        	{
	    		return NULL;
        	}
		}
        else{

        }
	delete configFile;
    }
    
    delete [] req;
	//cout << "fuck" <<endl;
    if(res == NULL)
    	return NULL;
    return res->c_str();
}

bool PlayCommand::fifo_send(char *command,char *read_fifo_path)
{
    int fd_read;  
    int fd_write;
    char return_value[255];

    fd_write = open(REQUEST_FIFO, O_WRONLY|O_NONBLOCK);
    if(write(fd_write, command, strlen(command)) == -1)
    {
        #ifdef DEBUG
	printf("Fuck!\n");
        #endif 
	return false;
    }
    
    close(fd_write);
   
    /*
    while(access(read_fifo_path, F_OK) != 0) //等待服务进程创建通信的fifo  
    {  
        sleep(1);  
    }
    */

    fd_read = open(read_fifo_path, O_RDONLY|O_NONBLOCK); 
    
    while((getline_from_fifo(fd_read,return_value,255))<=0);
    #ifdef DEBUG
    printf("return = %s\n",return_value);    
    #endif

    char *token=strtok(return_value," ");

    if(strcmp(token,"OK")==0)
    {
	return true;
    }else{
	return false;
    }

}

string PlayCommand::toString()
{
	return string("get the rstp url of file ") + request + "\n";
}
