#include"ConfigFile.h"
#include<iostream>

using namespace std;

int main()
{
	string s = "./ffserver.config";
	string s1 = "./abs130.avi";
	ConfigFile file =ConfigFile(s,s1);
	string s2= file.getConfigFileName();

	cout << s2 << endl;
	
	string size = string("1280x720");
	string bit = string("128");
	string frame = string("10");
	string s3 = string("quality:high,fps:22,bitRate:2048,size:1280x720");

	file.setVedioQuality(s3);	

	return 0;
}
