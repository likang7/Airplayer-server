#include"Quality.h"
#include <string.h>
#include <stdlib.h> 
#include <stdio.h>


//#define DEBUG

using std::string;

Quality::Quality(string video)
{
	v_file = video;
}

Quality::~Quality()
{

}

char* Quality::getSize()
{
    FILE *stream; 
    char buf[1024]; 
    char *p = new char[1024];
    char command[255];
	
    sprintf(command,"ffmpeg -i %s 2>&1 | grep 'Stream #0:0'|cut -d ',' -f 3 | cut -d '[' -f 1",v_file.c_str());
	
	#ifdef DEBUG
    printf("%s\n",command);
    #endif
	
    memset(buf,'\0',sizeof(buf));//初始化buf
    stream = popen(command,"r"); //将命令的输出 通过管道读取（“r”参数）到FILE* stream

    fread(buf,sizeof(char),sizeof(buf),stream); //将刚刚FILE* stream的数据流读取到buf中

    pclose(stream);
    strcpy(p,buf);
	
	return p;
}
