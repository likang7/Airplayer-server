INCLUDES = -I./live/UsageEnvironment/include -I./live/groupsock/include -I./live/liveMedia/include -I./live/BasicUsageEnvironment/include -Isrc/include
APCLUDES = -Isrc/include
#
##### Change the following for your environment:
COMPILE_OPTS =	$(APCLUDES) -I. -O2 -DSOCKLEN_T=socklen_t -D_LARGEFILE_SOURCE=1
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
VPATH = $(APHEAD):$(src)/mediaServer

APHEAD = $(src)/include
option = -O2 #-lpthread#-fprofile-arcs -ftest-coverage

AP_OBJS = 	$(objFile)/FormatFilter.o \
			$(objFile)/DownloadCommand.o \
			$(objFile)/GetSubtitlesCommand.o \
			$(objFile)/PreviewCommand.o \
			$(objFile)/HomeCommand.o \
		  	$(mediaServerObj)/MyRTSPServer.o  \
			$(objFile)/AirPlayerServer.o \
		  	$(objFile)/main.o \
			$(objFile)/Server.o $(objFile)/Command.o\
			$(objFile)/LsCommand.o $(objFile)/PlayCommand.o \
			$(objFile)/DescribeCommand.o $(objFile)/CommandFactory.o\
			$(objFile)/MediaInfo.o

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

#.$(CPP).$(OBJ):
#	$(CPLUSPLUS_COMPILER) -c $(CPLUSPLUS_FLAGS) $<

livemedia:
	cd $(LIVE_DIR); make

apserver: $(AP_OBJS)
	$(CPLUSPLUS_COMPILER) $(AP_OBJS) $(LIBS) ${CPLUSPLUS_FLAGS} -o apserver -lpthread
	
createDir:
	mkdir -p $(objFile)

$(objFile)/HomeCommand.o:$(src)/HomeCommand.cpp AirPlayerServer.h \
	HomeCommand.h Command.h
	$(CPLUSPLUS_COMPILER) -c $(src)/HomeCommand.cpp -o $@ $(CPLUSPLUS_FLAGS)

$(mediaServerObj)/MyRTSPServer.o:$(mediaServerSrc)/MyRTSPServer.cpp \
	MyRTSPServer.h
	$(CPLUSPLUS_COMPILER) $(INCLUDES) -c $(mediaServerSrc)/MyRTSPServer.cpp -o $@ ${CPLUSPLUS_FLAGS} 

$(objFile)/main.o: $(src)/main.cpp AirPlayerServer.h 
	$(CPLUSPLUS_COMPILER)  $(INCLUDES) -c $(src)/main.cpp -o $@ \
		${CPLUSPLUS_FLAGS} -lpthread

$(objFile)/AirPlayerServer.o: AirPlayerServer.h CommandFactory.h \
	Server.h $(src)/AirPlayerServer.cpp
	$(CPLUSPLUS_COMPILER)   -c $(src)/AirPlayerServer.cpp \
		-o $@ ${CPLUSPLUS_FLAGS} 
	
$(objFile)/Server.o: $(src)/Server.cpp Server.h
	$(CPLUSPLUS_COMPILER)   -c $(src)/Server.cpp -o $@ ${CPLUSPLUS_FLAGS} 
	
$(objFile)/Command.o: $(src)/Command.cpp Command.h
	$(CPLUSPLUS_COMPILER)   -c $(src)/Command.cpp -o $@ ${CPLUSPLUS_FLAGS} 
	
$(objFile)/LsCommand.o: $(src)/LsCommand.cpp Command.h \
		LsCommand.h FormatFilter.h
	$(CPLUSPLUS_COMPILER)   -c $(src)/LsCommand.cpp -o $@ ${CPLUSPLUS_FLAGS} 
	
$(objFile)/PlayCommand.o: $(src)/PlayCommand.cpp Command.h \
	PlayCommand.h Property.h MediaInfo.h AirPlayerServer.h
	$(CPLUSPLUS_COMPILER)  -c $(src)/PlayCommand.cpp -o $@ ${CPLUSPLUS_FLAGS} 

$(objFile)/DescribeCommand.o: $(src)/DescribeCommand.cpp Command.h \
	DescribeCommand.h AirPlayerServer.h
	$(CPLUSPLUS_COMPILER)   -c $(src)/DescribeCommand.cpp \
		-o $@ ${CPLUSPLUS_FLAGS} 
	
$(objFile)/CommandFactory.o: $(src)/CommandFactory.cpp \
	CommandFactory.h Command.h DescribeCommand.h PreviewCommand.h \
	PlayCommand.h LsCommand.h Command.h HomeCommand.h
	$(CPLUSPLUS_COMPILER)   -c $(src)/CommandFactory.cpp \
		-o $@ ${CPLUSPLUS_FLAGS}

$(objFile)/PreviewCommand.o:$(src)/PreviewCommand.cpp \
	Command.h PreviewCommand.h MediaInfo.h 
	$(CPLUSPLUS_COMPILER)   -c $(src)/PreviewCommand.cpp \
		-o $@ ${CPLUSPLUS_FLAGS}

$(objFile)/MediaInfo.o: $(src)/MediaInfo.cpp MediaInfo.h 
	$(CPLUSPLUS_COMPILER)  -c $(src)/MediaInfo.cpp -o $@ ${CPLUSPLUS_FLAGS}

$(objFile)/GetSubtitlesCommand.o: $(src)/GetSubtitlesCommand.cpp \
	GetSubtitlesCommand.h Command.h
	$(CPLUSPLUS_COMPILER)  -c $(src)/GetSubtitlesCommand.cpp \
		-o $@ ${CPLUSPLUS_FLAGS}

$(objFile)/DownloadCommand.o:$(src)/DownloadCommand.cpp \
	DownloadCommand.h Command.h
	$(CPLUSPLUS_COMPILER)  -c $(src)/DownloadCommand.cpp \
		-o $@ ${CPLUSPLUS_FLAGS}
		
$(objFile)/FormatFilter.o: $(src)/FormatFilter.cpp FormatFilter.h
	$(CPLUSPLUS_COMPILER)  -c $(src)/FormatFilter.cpp \
		-o $@ ${CPLUSPLUS_FLAGS}
	
clean:
	-rm -f $(objFile)/*.o
	-rm -f $(mediaServerObj)/*.o
	$(MAKE) clean -C $(LIVE_DIR)
	-rm -f apserver

