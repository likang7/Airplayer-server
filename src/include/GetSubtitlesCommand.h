#ifndef __GetSubtitlesCommand__H
#define __GetSubtitlesCommand__H

#include "Command.h"

class GetSubtitlesCommand : public Command
{
public:
	GetSubtitlesCommand(const char* _fp);
    virtual void execute(std::string& res);
    virtual std::string toString();
private:
 	const char* fp;
};

#endif