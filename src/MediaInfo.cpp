#include "MediaInfo.h"
#include <cstdio>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <unistd.h>

extern char* pathFFMPEG;

std::string getMediaName(const std::string& fp){
    size_t index = fp.find_last_of('/');
    if(index == std::string::npos)
        index = -1;
    index++;
    return fp.substr(index);
}

//read the encoding information about the file with ffprobe
int getMediainfo(const std::string& fp, MediaInfo& media)   
{   
    media.fp = fp;
    media.filename = getMediaName(fp);
    char buf[1024];   
    char ps[1024]={0};   
    FILE *ptr;   
    strcpy(ps, (std::string(pathFFMPEG) + "/ffmpeg -i \"" + fp + "\" 2>&1").c_str());   
    //execute the cmd and read it from the pipe with popen func
    if((ptr=popen(ps, "r"))!=NULL)   
    {   
        while(fgets(buf, 1024, ptr)!=NULL)   
        {  
            char* sub = buf;
            while(*sub == ' ')
                sub++; 
            char* tmp = NULL; ;
            if((tmp = strstr(sub, "Duration: ")) != NULL){
                tmp += strlen("Duration: ");
                const char* from = tmp;
                while(*tmp != ' ' && *tmp != ',' && *tmp != '\0' \
                    && *tmp != '\n' && *tmp != '.'){
                    tmp++;
                }
                media.duration.clear();
                media.duration.append(from, tmp - from);
            }
            else if((tmp = strstr(sub, "Video: ")) != NULL){
                //ignore the first two token ','
                while(*tmp++ != ',')
                    ;
                while(*tmp++ != ',')
                    ;
                while(*tmp == ' ')
                    tmp++;

                //get the solution information
                sscanf(tmp, "%dx%d", &media.width, &media.height);
                #ifdef DEBUG
                printf("***resolution = %dx%d\n", media.width, media.height);
                #endif
            }
            else
            {
                continue;
            }
        }   
        pclose(ptr);   
        ptr = NULL;   
        return 0;
    }   
    else  
    {   
        return -1;
    }   
}