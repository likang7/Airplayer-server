#ifndef __PREVIEWCOMMAND__H
#define __PREVIEWCOMMAND__H

#include "Command.h"
#include <string>

class PreviewCommand : public Command
{
public:
	PreviewCommand(const char* _request, int _clfd);
    virtual void execute(std::string& res);
    virtual std::string toString();
private:
	int parse(char* str);
 	const char* request;
 	std::string resolution;
 	int clfd;
};

#endif