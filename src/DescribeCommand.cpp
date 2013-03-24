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
#include <cstring>

//#define DEBUG
using std::string;
extern char* pathFFMPEG;

DescribeCommand::DescribeCommand(const char* _request) :
    request(_request)
{

}

/*
 *execute后返回一个视频文件的相关属性
 *如果无法获得属性，返回NULL
 */
void DescribeCommand::execute(std::string& res)
{
    if(!res.empty())
        return ;

    const char* fp = request;
    
    if(access(fp, R_OK|W_OK))
    {
	#ifdef DEBUG
    	printf("File does not exist!\n");
    #endif
	   return ;
    }
    
    //获取分辨率以及媒体长度
    getMediainfo(fp);
    //获取文件大小
    getFileSize(fp);


    res = string("resolution:") + resolution + ", duration:" + duration + ", size:" + filesize;
    
    return ;
}

string DescribeCommand::toString()
{
	return string("get the description of file ") + request + "\n";
}

//read the encoding information about the file with ffprobe
int DescribeCommand::getMediainfo(const std::string fp)   
{   
    char buf[1024];   
    char ps[1024]={0};   
    FILE *ptr;   
    strcpy(ps, (string(pathFFMPEG) + "/ffmpeg -i \"" + fp + "\" 2>&1").c_str());   
    //execute the cmd and read it from the pipe with popen func
    if((ptr=popen(ps, "r"))!=NULL)   
    {   
        while(fgets(buf, 1024, ptr)!=NULL)   
        {  
            char* sub = buf;
            while(*sub == ' ')
                sub++; 
            char* tmp = NULL; ;
            if((tmp = strstr(sub, "Duration: ")) != NULL){
                tmp += strlen("Duration: ");
                const char* from = tmp;
                while(*tmp != ' ' && *tmp != ',' && *tmp != '\0' \
                    && *tmp != '\n' && *tmp != '.'){
                    tmp++;
                }
                this->duration.append(from, tmp - from);
            }
            else if((tmp = strstr(sub, "Video: ")) != NULL){
                //ignore the first two token ','
                while(*tmp++ != ',')
                    ;
                while(*tmp++ != ',')
                    ;
                while(*tmp == ' ')
                    tmp++;

                //get the solution information
                const char* from = tmp;
                while((*tmp >= '0' && *tmp <= '9') || *tmp == 'x')
                    tmp++;
                this->resolution.append(from, tmp - from);
                #ifdef DEBUG
                printf("***resolution = %s\n", resolution.c_str());
                #endif
            }
            else
            {
                continue;
            }
        }   
        pclose(ptr);   
        ptr = NULL;   
        return 0;
    }   
    else  
    {   
        return -1;
    }   
} 

int DescribeCommand::getFileSize(const std::string fp){
    char buf[1024];   
    char ps[1024]={0};   
    FILE *ptr;   
    strcpy(ps, (string("ls -lh \"") + fp + "\" |cut -d ' ' -f 5").c_str());   
    //execute the cmd and read it from the pipe with popen func
    if((ptr=popen(ps, "r"))!=NULL){
        while(fgets(buf, 1024, ptr)!=NULL)
            filesize += buf;
        for(string::iterator it = filesize.end() - 1; it != filesize.begin(); it--){
            if(*it == '\n' || *it == '\r'){
                it = filesize.erase(it);
            }
            else
                break;
        }
        pclose(ptr);   
        ptr = NULL;   
        return 0;
    }
    else{
        return -1;
    }
}