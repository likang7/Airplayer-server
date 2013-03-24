#include "CommandFactory.h"
#include "LsCommand.h"
#include "PlayCommand.h"
#include "DescribeCommand.h"
#include "HomeCommand.h"
#include "PreviewCommand.h"
#include "GetSubtitlesCommand.h"
#include "DownloadCommand.h"
#include <stdio.h>
using std::string;

/*
 *返回Command的对应实例
 *type为Command的类型
 *request为请求
 *失败返回NULL
 *注意获取实例者有释放内存的责任
 */
Command* CommandFactory::create(const string type, const char* request, int clfd)
{
	Command* command = NULL;
	if(type == "ls"){
		command = new LsCommand(request);
	}
	else if(type == "play"){
		command = new PlayCommand(request);
	}
	else if(type == "describe"){
		command = new DescribeCommand(request);
	}
	else if(type == "home"){
		command = new HomeCommand();
	}
	else if(type == "preview"){
		command = new PreviewCommand(request, clfd);
	}
	else if(type == "getsubtitles"){
		command = new GetSubtitlesCommand(request);
	}
	else if(type == "download"){
		command = new DownloadCommand(request, clfd);
	}
	else{
		command = NULL;
	}
	return command;
}
