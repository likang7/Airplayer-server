#include "LsCommand.h"
#include "FormatFilter.h"
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

}

/*
 *path为目录的路径，以字符串形式返回该目录下的文件信息
 *失败返回NULL
 */
void LsCommand::execute(std::string& res)
{
	//1. 如果曾经执行过该命令，直接返回结果
    if(!res.empty())
        return ;
        
	//2. 检验路径是否存在且能打开，并且是否为一个目录
    struct stat _stat;
    DIR *dp;
    if(lstat(path, &_stat) < 0 //|| !S_ISDIR(_stat.st_mode) 
    	|| ((dp = opendir(path)) == NULL))
    {
    	return ;
    }
    
    //3. 读取该目录下的文件，将结果存放在res中
    struct dirent* dirp;
    char buf[BUFSIZE];
	//该目录下的文件数量
	int num = 0;
    while((dirp = readdir(dp)) != NULL){
    	if(dirp->d_name[0] == '.' /*&& dirp->d_name[1] != '.'*/)
            continue;

    	sprintf(buf, "%s", dirp->d_name);
    	
    	//给客户端增加文件信息，0表示是文件夹，1表示不是文件夹
    	if(lstat((string(path) + "/" + dirp->d_name).c_str(), &_stat) >= 0)
    	{
    		if(opendir((string(path) + "/" + dirp->d_name).c_str()) != NULL)
    		{
    			res.append("\n");
                res.append(buf);
                res.append(",0");
			}
			else 
			{
                const char* extension = strrchr(dirp->d_name, '.');
                if(extension == NULL || \
                    FormatFilter::isSupportFormat(extension) == false)
                    continue;
				res.append("\n");
                res.append(buf);
                res.append(",1");
			}
    	}
		num++;
    }
	sprintf(buf, "%d", num);
    res = string(buf) + res;
	
    //4. 返回结果
    return ;
}

string LsCommand::toString()
{
	return string("list the directory at ") + path;
}
