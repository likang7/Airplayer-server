#include "HomeCommand.h"
#include "AirPlayerServer.h"
using std::string;

HomeCommand::HomeCommand(const char* _path) :
    path(_path)
{

}

HomeCommand::~HomeCommand()
{

}

void HomeCommand::execute(std::string& res)
{
	//1. 如果曾经执行过该命令，直接返回结果
    if(!res.empty())
        return ;
        
    AirPlayerServer* server = AirPlayerServer::getInstance();
    res = server->getContent_dir();
    
    return ;
}

string HomeCommand::toString()
{
	return string("sent the home directory.") ;
}

