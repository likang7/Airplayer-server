#ifndef __PROPERTY_H__
#define __PPROPERTY_H__

#include<string>

enum Option{
    DIRECT,
    HIGH,
    MEDIUM,
    LOW,
};

struct Property{
	//视频质量。是对下面的视频参数的概括性描述
	Option qual;
	//帧率
	std::string fps;
	//分辨率
	unsigned width;
	unsigned heigt;
	//比特率
	std::string bitRate;

	Property(){
		qual = DIRECT;
	}
};

#endif
