#include "Command.h"

Command::Command() :
    res(NULL)
{

}

Command::~Command()
{
    delete res;
}
