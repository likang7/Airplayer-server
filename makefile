INCLUDES = -I./live/UsageEnvironment/include -I./live/groupsock/include -I./live/liveMedia/include -I./live/BasicUsageEnvironment/include
#
##### Change the following for your environment:
COMPILE_OPTS =		$(INCLUDES) -I. -O2 -DSOCKLEN_T=socklen_t -D_LARGEFILE_SOURCE=1
C =			c
C_COMPILER =		cc
C_FLAGS =		$(COMPILE_OPTS) $(CPPFLAGS) $(CFLAGS)
CPP =			cpp
CPLUSPLUS_COMPILER =	c++
CPLUSPLUS_FLAGS =	$(COMPILE_OPTS) -Wall -DBSD=1 $(CPPFLAGS) $(CXXFLAGS)
OBJ =			o
LINK =			c++ -o
LINK_OPTS =		-L. $(LDFLAGS)
CONSOLE_LINK_OPTS =	$(LINK_OPTS)
LIBRARY_LINK =		ar cr 
LIBRARY_LINK_OPTS =	
LIB_SUFFIX =			a
LIBS_FOR_CONSOLE_APPLICATION =
LIBS_FOR_GUI_APPLICATION =
EXE =
##### End of variables to change
src = src
mediaServerSrc = $(src)/mediaServer
objFile = src
mediaServerObj = $(mediaServerSrc)
option = -O2 -lpthread#-fprofile-arcs -ftest-coverage

AP_OBJS = $(objFile)/HomeCommand.o $(mediaServerObj)/DynamicRTSPServer.o  $(objFile)/AirPlayerServer.o $(objFile)/main.o $(objFile)/Server.o $(objFile)/Command.o\
	$(objFile)/LsCommand.o $(objFile)/PlayCommand.o $(objFile)/DescribeCommand.o $(objFile)/CommandFactory.o\
        $(objFile)/Quality.o $(objFile)/Hquality.o $(objFile)/Mquality.o $(objFile)/Lquality.o $(objFile)/ConfigFile.o

all:livemedia apserver

LIVE_DIR = live

LIB_Dir = $(LIVE_DIR)

USAGE_ENVIRONMENT_LIB = $(LIB_Dir)/UsageEnvironment/libUsageEnvironment.$(LIB_SUFFIX)
BASIC_USAGE_ENVIRONMENT_LIB = $(LIB_Dir)/BasicUsageEnvironment/libBasicUsageEnvironment.$(LIB_SUFFIX)
LIVEMEDIA_LIB = $(LIB_Dir)/liveMedia/libliveMedia.$(LIB_SUFFIX)
GROUPSOCK_LIB = $(LIB_Dir)/groupsock/libgroupsock.$(LIB_SUFFIX)
LOCAL_LIBS =	$(LIVEMEDIA_LIB) $(GROUPSOCK_LIB) \
		$(BASIC_USAGE_ENVIRONMENT_LIB) $(USAGE_ENVIRONMENT_LIB)
LIBS =		$(LOCAL_LIBS) $(LIBS_FOR_CONSOLE_APPLICATION)


livemedia:
	cd $(LIVE_DIR); make

apserver: createDir $(AP_OBJS)
	g++ $(AP_OBJS) $(LIBS) ${option} -o apserver
	
createDir:
	mkdir -p $(objFile)

$(objFile)/HomeCommand.o:
	g++ -c $(src)/HomeCommand.cpp -o $(objFile)/HomeCommand.o $(option)

$(mediaServerObj)/DynamicRTSPServer.o:
	g++ $(INCLUDES) -c $(mediaServerSrc)/DynamicRTSPServer.cpp -o $(mediaServerObj)/DynamicRTSPServer.o

$(objFile)/main.o: 
	g++ $(INCLUDES) -c $(src)/main.cpp -o $(objFile)/main.o ${option} 

$(objFile)/AirPlayerServer.o: 
	g++ -c $(src)/AirPlayerServer.cpp -o $(objFile)/AirPlayerServer.o ${option} 
	
$(objFile)/Server.o: 
	g++ -c $(src)/Server.cpp -o $(objFile)/Server.o ${option} 
	
$(objFile)/Command.o:
	g++ -c $(src)/Command.cpp -o $(objFile)/Command.o ${option} 
	
$(objFile)/LsCommand.o:
	g++ -c $(src)/LsCommand.cpp -o $(objFile)/LsCommand.o ${option} 
	
$(objFile)/PlayCommand.o:
	g++ -c $(src)/PlayCommand.cpp -o $(objFile)/PlayCommand.o ${option} 

$(objFile)/DescribeCommand.o:
	g++ -c $(src)/DescribeCommand.cpp -o $(objFile)/DescribeCommand.o ${option} 
	
$(objFile)/CommandFactory.o:
	g++ -c $(src)/CommandFactory.cpp -o $(objFile)/CommandFactory.o ${option}

$(objFile)/Quality.o:
	g++ -c $(src)/Quality.cpp -o $(objFile)/Quality.o ${option}

$(objFile)/Hquality.o:
	g++ -c $(src)/Hquality.cpp -o $(objFile)/Hquality.o ${option}

$(objFile)/Mquality.o:
	g++ -c $(src)/Mquality.cpp -o $(objFile)/Mquality.o ${option} 

$(objFile)/Lquality.o:
	g++ -c $(src)/Lquality.cpp -o $(objFile)/Lquality.o ${option} 

$(objFile)/ConfigFile.o: 
	g++ -c $(src)/ConfigFile.cpp -o $(objFile)/ConfigFile.o ${option} 
	
clean:
	-rm $(objFile)/*.o
#	cd $(LIVE_DIR); make clean

