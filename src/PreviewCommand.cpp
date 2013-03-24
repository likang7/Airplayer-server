#include "PreviewCommand.h"
#include "MediaInfo.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>
using std::string;
using std::stringstream;

extern char* pathFFMPEG;
extern int getMediainfo(const std::string& fp, MediaInfo& media);

PreviewCommand::PreviewCommand(const char* _request, int _clfd) : 
	request(_request), clfd(_clfd)
{
	resolution = "320x160";
}

int PreviewCommand::parse(char* str){
    char key[100];
    char value[100];
    while(*str){
        int i;
        for(i = 0; *str != ':'; i++){
            key[i] = *str++;
        }
        key[i] = '\0';
        str++;
        for(i = 0; *str != '\0' && *str != ','; i++){
            value[i] = *str++;
        }
        value[i] = '\0';
        if(*str != '\0')
            str++;
        #ifdef DEBUG
        printf("key = %s, value = %s\n", key, value);
        #endif
        if(strcmp(key, "quality") == 0){
        }
        else if(strcmp(key, "resolution") == 0){
        	resolution = value;
        }
        else if(strcmp(key, "bitrate") == 0){

        }
        else if(strcmp(key, "fps") == 0){

        }
    }
    return 0;
}

void PreviewCommand::execute(std::string& res)
{
    if(!res.empty())
        return ;

	/*
     *先解析request，提取出文件路径和编码请求
     */
    char* buf = strchr((char*)request, ',');
	string fp;
	fp.append(request, buf - request);
	buf++;

	parse(buf);

    MediaInfo media;
    string itoffset = "60";
    if(getMediainfo(fp, media) >= 0){
        string duration = media.duration;
        int hh, mm, ss;
        sscanf(duration.c_str(), "%d:%d:%d", &hh, &mm, &ss);
        int len = hh * 3600 + mm * 60 + ss;
        len /= 3;
        stringstream sst;
        sst << len;
        itoffset = sst.str();
    }


	string output = fp + ".jpg";

	string cmd = string(pathFFMPEG) + "/ffmpeg -ss " +  itoffset + \
        " -y -i \"" + fp + "\" -vframes 1 \
		-f mjpeg -s " + resolution +  " -an \"" + output + "\"" + " 2>/dev/null";

	int err = system(cmd.c_str());

	if(err != 0){
		return ;
	}

	if (FILE *fp = fopen(output.c_str(), "r"))
	{
		/*****
		return ok message first
		****/
		char buf[BUFSIZ];
		//sprintf(buf, "OK\n");
		//send(clfd, buf, strlen("OK\n"), 0);
		while (size_t len = fread(buf, 1, sizeof(buf), fp))
			send(clfd, buf, len, 0);
		fclose(fp);
        remove(output.c_str());
	}

	close(clfd);
	res = string("@@@@@@");
	return ;
}

std::string PreviewCommand::toString()
{
	return string("get the preview picture of ") + request + "\n";
}