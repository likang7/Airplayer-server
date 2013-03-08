#include "DescribeCommand.h"
#include "AirPlayerServer.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>
#include <unistd.h>

//#define DEBUG
using std::string;

DescribeCommand::DescribeCommand(const char* _request) :
    request(_request)
{

}

/*
 *execute后返回一个视频文件的相关属性
 *如果无法获得属性，返回NULL
 */
const char* DescribeCommand::execute()
{
    if(res != NULL)
        return res->c_str();
    
    /*
     *以下部分需要先解析request，提取出文件路径
     */
    char* req = new char[200];
    char* token;
    char* resolution;
    char* duration;
    char* size;
    char command[255];
    char temp[255];
    int i;

    strcpy(req,request);
    token=strtok(req," ");
    if(token == NULL)
    	return NULL;
    token=strtok(req," ");
    
    #ifdef DEBUG
    printf("%s\n",token);
    #endif
    
    if(access(req, R_OK|W_OK))
    {
	#ifdef DEBUG
    	printf("File does not exist!\n");
    	#endif
	return NULL;
    }
    //获取分辨率
    sprintf(command,"ffmpeg -i %s 2>&1 | grep 'Stream #0:0'|cut -d ',' -f 3 | cut -d '[' -f 1",req);
   	
    #ifdef DEBUG
    printf("%s\n",command);
    #endif
    
    resolution = getSomething(command);
    i = strlen(resolution);
    resolution[i-1] = '\0';
    #ifdef DEBUG
    printf("%s %d\n",resolution,strlen(resolution));
    #endif
    
    //获取视频长度
    memset(command,'\0',sizeof(command));
    sprintf(command,"ffmpeg -i %s 2>&1 | grep 'Duration' | cut -d ' ' -f 4 | sed s/,//",req);
   	
    #ifdef DEBUG
    printf("%s\n",command);
    #endif
    
    duration = getSomething(command);
    token=strtok(duration,".");

    #ifdef DEBUG
    printf("%s %d\n",duration,strlen(duration));
    #endif
    
    //获取文件大小
    memset(command,'\0',sizeof(command));
    sprintf(command,"ls -lh %s|cut -d ' ' -f 5",req);
   	
    #ifdef DEBUG
    printf("%s\n",command);
    #endif
    
    size = getSomething(command);
    i = strlen(size);
    size[i-1] = '\0';

    #ifdef DEBUG
    printf("%s %d\n",size,strlen(size));
    #endif

    sprintf(temp,"resolution:%s, duration:%s, size:%s\n",resolution,duration,size);
    #ifdef DEBUG
    printf("%s\n",temp);
    #endif

    res = new string(temp);
    
    delete resolution,duration,size,req;
    if(res == NULL)
    	return NULL;
    return res->c_str();
}

char* DescribeCommand::getSomething(char *command)
{
    FILE *stream; 
    char buf[1024]; 
    char *p = new char[1024];
    memset(buf,'\0',sizeof(buf));//初始化buf
    stream = popen(command,"r"); //将命令的输出 通过管道读取（“r”参数）到FILE* stream

    fread(buf,sizeof(char),sizeof(buf),stream); //将刚刚FILE* stream的数据流读取到buf中

    pclose(stream);
    strcpy(p,buf);
    return p;
}

string DescribeCommand::toString()
{
	return string("get the description of file ") + request + "\n";
}
