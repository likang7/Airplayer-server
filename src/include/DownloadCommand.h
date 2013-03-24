#ifndef __DOWNLOADCOMMAND__H
#define __DOWNLOADCOMMAND__H
#include "Command.h"

class DownloadCommand : public Command
{
public:
	DownloadCommand(const char* _fp, int _clfd);
    virtual void execute(std::string& res);
    virtual std::string toString();
private:
 	const char* fp;
 	int clfd;
};

#endif