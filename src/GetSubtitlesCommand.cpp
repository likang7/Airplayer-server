#include "GetSubtitlesCommand.h"
#include <cstring>
#include <dirent.h>
#include <string>
#include <cstdio>
using std::string;

//#define DEBUG 

GetSubtitlesCommand::GetSubtitlesCommand(const char* _fp) : fp(_fp)
{

}

/*
 *return the paths of the subtitle files, one path each line
 * (eg, path1\npath2)
 */
void GetSubtitlesCommand::execute(std::string& res)
{
	if(!res.empty())
		return;

	//the start index of the filename
	char* index = strrchr((char*)fp, '/');
	if(index == NULL){
		printf("ERROR, uncorrect file path : %s\n", res.c_str());
		return;
	}

	string wd(fp, index - fp); //the path of the working directory
	char* end = strrchr((char*)fp , '.');// the end of the filename
	if(end == NULL){
		end = (char*)fp + strlen(fp);
	}
	string filename(index + 1, end - index - 1);//the name of the file
	if(!strcmp(end, ".ts")){
		size_t pos = filename.find_last_of('.');
		if(pos != string::npos && !filename.compare(pos, strlen(".___DIRECT"), ".___DIRECT")){
			// erase the real suffix
			filename.erase(pos);
			pos = filename.find_last_of('.'); 
			filename.erase(pos);
		}
	}

	DIR* dp = opendir(wd.c_str());
	if(dp == NULL){
		printf("ERROR, cannot access directory %s\n", wd.c_str());
		return;
	}
	struct dirent* dirp;

	while((dirp = readdir(dp)) != NULL){
		const char* name = dirp->d_name;
		const char* extension = strrchr(name, '.');
		if(extension == NULL)
			continue;
		if(strncmp(filename.c_str(), name, filename.size()) == 0 \
			 && strcmp(extension, ".srt") == 0){
			res += wd + "/" + dirp->d_name + "\n";
		}
	}
	if(res.empty())
		res = string("@@@@@@");
	return ;
}

std::string GetSubtitlesCommand::toString()
{
	return string("get the subtitle files at ") + fp;
}
