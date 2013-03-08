#ifndef __QUALITY_H__
#define __QUALITY_H__

#include<string>
#include"Property.h"


//策略基类
class Quality
{
	private:
		//视频文件名
		std::string v_file;
	protected:
		//获取原分辨率
		char* getSize();
	
	public:
		Quality(std::string);
		~Quality();
		//设置视频参数的虚函数
		virtual struct Property * set(struct Property *){};

		
		
};


#endif

