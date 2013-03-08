#include "LsCommand.h"
#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include <dirent.h>
using namespace std;
#define BUFSIZE 4096

LsCommand::LsCommand(const char* _path) :
    path(_path)
{

}

LsCommand::~LsCommand()
{
	delete res;
	res = NULL;
}

/*
 *path为目录的路径，以字符串形式返回该目录下的文件信息
 *失败返回NULL
 */
const char* LsCommand::execute()
{
	//1. 如果曾经执行过该命令，直接返回结果
    if(res != NULL)
        return res->c_str();
        
	//2. 检验路径是否存在且能打开，并且是否为一个目录
    struct stat _stat;
    DIR *dp;
    if(lstat(path, &_stat) < 0 || !S_ISDIR(_stat.st_mode) 
    	|| ((dp = opendir(path)) == NULL))
    {
    	return NULL;
    }
    
    //3. 读取该目录下的文件，将结果存放在res中
    res = new string();
    struct dirent* dirp;
    char buf[BUFSIZE];
	//该目录下的文件数量
	int num = 0;
    while((dirp = readdir(dp)) != NULL){
    	if(dirp->d_name[0] == '.' && dirp->d_name[1] != '.')
            continue;
    	sprintf(buf, "%s", dirp->d_name);
    	*res += "\n";
    	*res += buf;
    	
    	//给客户端增加文件信息，0表示是文件夹，1表示不是文件夹
    	if(lstat((string(path) + "/" + dirp->d_name).c_str(), &_stat) >= 0)
    	{
    		if(S_ISDIR(_stat.st_mode))
    		{
    			*res += ",0";
			}
			else 
			{
				*res += ",1";
			}
    	}
		num++;
    }
	sprintf(buf, "%d", num);
    *res = string(buf) + *res;
	
    //4. 返回结果
    return res->c_str();
}

string LsCommand::toString()
{
	return string("list the directory at ") + path;
}
