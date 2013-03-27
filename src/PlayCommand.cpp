#include "PlayCommand.h"
#include "AirPlayerServer.h"
#include <cstdio>
#include <cstring>
#include <sstream>
#include <pthread.h>
#include <cstdlib>
#include <unistd.h>

using namespace std;
const int BLOCKSIZE = 16;
extern int rtspServerPortNum;
extern char* pathFFMPEG;
extern int getMediainfo(const std::string& fp, MediaInfo& media); 

#define DEBUG

//request的格式为filepath, 转码请求(如quality:xxx, fps:xxx)
PlayCommand::PlayCommand(const char* _request) :
    request(_request)
{
    media.duration = "00:00:00";
    media.fp.clear();
    media.height = 480;
    media.width = 800;
}

void setQuality(const char* q, Property* p)
{
    if(strcmp(q, "high") == 0){
        p->qual = HIGH;
    }
    else if(strcmp(q, "medium") == 0){
        p->qual = MEDIUM;
    }
    else if(strcmp(q, "low") == 0){
        p->qual = LOW;
    }
    else{
        p->qual = DIRECT;
    }
}

//
void fitDeviceResolution(Property* p, MediaInfo& media){
    if(media.width <= p->width && media.height <= p->heigt){
        #ifdef DEBUG
        printf("condition 1\n");
        #endif
        p->width = media.width;
        p->heigt = media.height;
    }
    else if(media.width <= p->width && media.height > p->heigt){
        if(media.width <= p->heigt && media.height <= p->width){
            #ifdef DEBUG
            printf("condition 2\n");
            #endif
            p->width = media.width;
            p->heigt = media.height;
        }
        else{
            #ifdef DEBUG
            printf("condition 3\n");
            #endif
            p->width = 1.0 * media.width * p->heigt / media.height;
        }
    }
    else if(media.width >= p->width && media.height < p->heigt){
        #ifdef DEBUG
        printf("condition 4\n");
        #endif
        p->heigt = 1.0 * media.height * p->width / media.width;
    }
    else{
        #ifdef DEBUG
        printf("condition 5\n");
        #endif
        p->heigt = p->width * media.height / media.width;
        /*
        float ratio = 1.0 * media.width / media.height;
        float scale = 1.0 * media.width / p->width;
        if(ratio > 0.01 && p->width / ratio > p->heigt){
            p->width = ratio * p->heigt;
        }
        */
    }
    p->width = (int)((float)p->width / BLOCKSIZE + 0.5) * BLOCKSIZE;
    p->heigt = (int)((float)p->heigt / BLOCKSIZE + 0.5) * BLOCKSIZE; 

    #ifdef DEBUG
    printf("width = %d, height = %d\n", p->width, p->heigt);
    #endif
}

int PlayCommand::parse(char* str, Property* p){
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
            setQuality(value, p);
        }
        else if(strcmp(key, "resolution") == 0){
            sscanf(value, "%dx%d", &p->width, &p->heigt);
            fitDeviceResolution(p, this->media);
        }
        else if(strcmp(key, "bitrate") == 0){
            p->bitRate = value;
        }
        else if(strcmp(key, "fps") == 0){
            p->fps = value;
        }
        //str += strlen(key) + strlen(value) + 1;
        //if(*str == ',')
            //str++;
    }
    return 0;
}

void* mediaConvert(void *arg)
{
    string cmd = *(string*)arg;
    system(cmd.c_str());

    string fp;
    if(cmd[cmd.size()-1] == '"')
    {
        cmd.erase(cmd.size() - 1);
        size_t pos = cmd.find_last_of('"');
        fp = cmd.substr(pos + 1);
    }
    else
        fp = cmd.substr(cmd.find_last_of(' ') + 1);

    remove((fp + "x").c_str());
    AirPlayerServer* server = AirPlayerServer::getInstance();
    server->eraseConvertingMedia(fp);

    delete (string*)arg;
    return (void*)0;
}

bool PlayCommand::checkRewrite(const std::string& fp)
{
    if(access(fp.c_str(), R_OK)){
        return true;
    }
    MediaInfo tmpMedia;
    getMediainfo(fp, tmpMedia);
    const string index = fp + "x";

    int hh, mm, ss;
    sscanf(media.duration.c_str(), "%d:%d:%d", &hh, &mm, &ss);
    int len1 = hh * 3600 + mm * 60 + ss;
    sscanf(tmpMedia.duration.c_str(), "%d:%d:%d", &hh, &mm, &ss);
    int len2 = hh * 3600 + mm * 60 + ss;
    if(abs(len1 - len2) > 10){
        remove(index.c_str());
        return true;
    } 
/*
    getMediainfo(index, tmpMedia);
    sscanf(tmpMedia.duration.c_str(), "%d:%d:%d", &hh, &mm, &ss);
    len2 = hh * 3600 + mm * 60 + ss;
    if(abs(len1 - len2) > 10){
        remove(index.c_str());
    }
*/
    return false;
}

/*
 *execute后返回结果为一个可访问的rtsp地址。
 *如果无法获得rtsp地址，返回NULL
 */
void PlayCommand::execute(std::string& res)
{
    if(!res.empty())
        return ;
    /*
     *先解析request，提取出文件路径和编码请求
     *返回rtsp地址
     */
    char* buf = strchr((char*)request, ',');
    media.fp.append(request, buf - request);
    getMediainfo(media.fp, media);

    buf++;
    printf("buf = %s\n", buf);
    Property p;
    p.width = media.width;
    p.heigt = media.height;

    parse(buf, &p);
    AirPlayerServer* server = AirPlayerServer::getInstance();

    string* cmd = new string;
    string path = media.fp;
    if(p.qual == DIRECT){
        bool r = checkRewrite(path);
        if(r)
        {
            remove((path + "___DIRECT.ts").c_str());
        }
        //*res = string("rtsp://") + server->getLocalIP() + ":8554/" + media.fp;
    }
    else if(p.qual == HIGH){
        path = media.fp + ".___HIGH.ts";
        bool r = checkRewrite(path);
        string shouldRewrite;
        if(r){
            shouldRewrite = " -y ";

            stringstream resolution;
            resolution << p.width << "x" << p.heigt;

            *cmd = string(pathFFMPEG) + "/ffmpeg -i \"" + media.fp + "\"" + shouldRewrite + " -vcodec libx264 \
                -preset medium -threads 2 -acodec copy -s " + resolution.str() + " \"" + 
                media.fp + ".___HIGH.ts\"";
        }
    }
    else if(p.qual == MEDIUM){
        path = media.fp + ".___MEDIUM.ts";
        bool r = checkRewrite(path);
        string shouldRewrite;
        if(r){
            shouldRewrite = " -y ";
        
            stringstream resolution;
            resolution << p.width << "x" << p.heigt;
            *cmd = string(pathFFMPEG) + "/ffmpeg -i \"" + media.fp + "\""+ shouldRewrite + " -vcodec libx264 \
                -preset veryfast -threads 2 -acodec aac -strict experimental -ab \
                320k -s " + resolution.str() + " \"" + media.fp + ".___MEDIUM.ts\"";
        }
    }
    else if(p.qual == LOW){
        path = media.fp + ".___LOW.ts";
        bool r = checkRewrite(path);
        string shouldRewrite;
        if(r){
            shouldRewrite = " -y ";

            stringstream resolution;
            resolution << p.width << "x" << p.heigt;
            *cmd = string(pathFFMPEG) + "/ffmpeg -i \"" + media.fp + "\""+ shouldRewrite + " -vcodec libx264 \
                -preset ultrafast -tune fastdecode -crf 29 -threads 2 \
                -acodec aac -strict experimental -ab \
                192k -s " + resolution.str() + " \"" + media.fp + ".___LOW.ts\"";
        }
    }
    if(!cmd->empty() && server->addConvertingMedia(path)){
        pthread_t tid;
        int err = pthread_create(&tid, NULL, mediaConvert, (void*)cmd);
        if(err != 0){
            printf("cannot launch ffmpeg.\n");
            server->eraseConvertingMedia(path);
            delete cmd;
            return ;
        }
        //sleep(1);
    }
    stringstream ssportNum;
    ssportNum << ":" << rtspServerPortNum << "/";
    res = string("rtsp://") + server->getLocalIP() + ssportNum.str() + path;
    #ifdef DEBUG
    printf("play %s\n", res.c_str());
    #endif
}

string PlayCommand::toString()
{
	return string("get the rstp url of file ") + request + "\n";
}
