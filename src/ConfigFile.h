#ifndef __CONFIGFILE_H__
#define __CONFIGFILE_H__

#include"Hquality.h"
#include"Mquality.h"
#include"Lquality.h"
#include<string>

/*
 * 配置文件类，读取和修改相关视频属性
 */
class ConfigFile
{
	public:
		//构造函数
		ConfigFile(std::string confFile,std::string videoName);
		//析构函数
		~ConfigFile();
		//获取配置文件名
		std::string getConfigFileName();
		//获取ffserverHTTP服务器监听的端口号
		std::string getHTTPServerPort();
		//获取ffserverRTSP服务器监听的端口号
		std::string getRTSPServerPort();
		//在获取到要设置的视频质量后，使用策略模式获取相对应的策略对参数进行设置
		void setVedioQuality(std::string &s);
		//是否直接播放
		bool getIsDirect();
		//
		const std::string& getQuality();

	private:
		//初始化成员变量
		void init();             
		//获取配置文件中指定属性c的值，s为存储配置文件内容的字符串
		std::string getValue(char* c,std::string &s);
		//设置配置文件中指定属性c的值为value，s为存储配置文件内容的字符串
		bool setValue(char* c, std::string &value, std::string &s);
		//回写视频流参数：VideoSize，VideoFrameRate，VideoBitRate
		void writebackProperty();
        	//解析"quality:high,fps:30,..."类型的字符串
		void parse(std::string &s);
		//在配置文件中追加流标签信息
                void add(char *c);
		//解析"quality:high"类型的属性：属性值对
		void parseString(std::string &);
		//视频质量的相关属性，具体见Property.h文件
		struct Property p;
		//ffserverHTTP服务器监听的端口号
		std::string HTTPServerPort;
		//ffserverRTSP服务器监听的端口号
		std::string RTSPServerPort;
		//视频文件名
		std::string videoName;
		//是否直接播放
      		bool isDirect;
		//配置文件名
		std::string confFile;
};

#endif

