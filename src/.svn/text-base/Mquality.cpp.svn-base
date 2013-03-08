#include"Mquality.h"
#include"Quality.h"
#include <string.h>
#include <stdlib.h> 
#include <stdio.h>

#define DEBUG

Mquality::Mquality(string video):Quality(video)
{
	
}

Mquality::~Mquality()
{
}

struct Property* Mquality::set(struct Property *p)
{
	(*p).fps = string("30");
	//(*p).size = string("1280x720");
	(*p).bitRate = string("1024");
	
	char* size = getSize();
	
	#ifdef DEBUG
	printf("%s\n",size);
	#endif
	const char* high = strtok(size,"x");
	if(high == NULL)
    	return NULL;
		
	#ifdef DEBUG
	printf("%s\n",size);
	#endif
	
    const char* width = strtok(NULL,"x");
	if(width == NULL)
    	return NULL;
		
	#ifdef DEBUG
	printf("%s\n",width);
	#endif
	
	
	int h = atoi(high);
	int w = atoi(width);
	
	h = (int)(((float)h*0.7)/16)*16;
	w = (int)(((float)w*0.7)/16)*16;

	#ifdef DEBUG
	printf("%dx%d\n",h,w);
	#endif
	char s[15];
	
	sprintf(s,"%dx%d",h,w);
   	
    #ifdef DEBUG
    printf("%s\n",s);
    #endif
	
	(*p).size = string(s);
	return p;
}
