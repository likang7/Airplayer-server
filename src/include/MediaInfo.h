#ifndef __MEDIAINFO_H__
#define __MEDIAINFO_H__

#include <string>

class MediaInfo{
public:
    std::string fp;//file path
    std::string filename;//filename
    unsigned width;
    unsigned height;
    std::string duration;

};

int getMediainfo(const std::string& fp, MediaInfo& media);

#endif