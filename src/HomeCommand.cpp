#include "HomeCommand.h"
#include "AirPlayerServer.h"
using std::string;

HomeCommand::HomeCommand(const char* _path) :
    path(_path)
{

}

HomeCommand::~HomeCommand()
{
	delete res;
	res = NULL;
}

const char* HomeCommand::execute()
{
	//1. 如果曾经执行过该命令，直接返回结果
    if(res != NULL)
        return res->c_str();
        
    res = new string();
    AirPlayerServer* server = AirPlayerServer::getInstance();
    *res = server->getContent_dir();
    
    return res->c_str();
}

string HomeCommand::toString()
{
	return string("sent the home directory.") ;
}

