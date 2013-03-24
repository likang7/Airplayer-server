#include "DownloadCommand.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>

using std::string;

DownloadCommand::DownloadCommand(const char* _fp, int _clfd) 
	: fp(_fp), clfd(_clfd)
{

}

void DownloadCommand::execute(std::string& res)
{
	if(!res.empty())
		return;

	if (FILE *pf = fopen(fp, "r"))
	{
		/*****
		return ok message first
		****/
		char buf[BUFSIZ];
		//sprintf(buf, "OK\n");
		//send(clfd, buf, strlen("OK\n"), 0);
		while (size_t len = fread(buf, 1, sizeof(buf), pf))
			send(clfd, buf, len, 0);
		fclose(pf);
	}

	close(clfd);

	res = string("@@@@@@");
	return ;
}

std::string DownloadCommand::toString()
{
	return string("download the file at the path ") + fp + "\n";
}
