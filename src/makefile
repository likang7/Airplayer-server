INCLUDES = -Iinclude
MediaServerIncludes = -ImediaServer -I../live/UsageEnvironment/include -I../live/groupsock/include -I../live/liveMedia/include -I../live/BasicUsageEnvironment/include
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
src = .
objFile = .
option = -O2 -lpthread#-fprofile-arcs -ftest-coverage

AP_OBJS = $(objFile)/PreviewCommand.o $(objFile)/HomeCommand.o  $(objFile)/AirPlayerServer.o $(objFile)/main.o $(objFile)/Server.o $(objFile)/Command.o\
	$(objFile)/LsCommand.o $(objFile)/PlayCommand.o $(objFile)/DescribeCommand.o $(objFile)/CommandFactory.o

all:$(AP_OBJS)

$(objFile)/HomeCommand.o:
	g++  $(INCLUDES) -c $(src)/HomeCommand.cpp -o $(objFile)/HomeCommand.o $(option)

$(objFile)/main.o: 
	g++ $(MediaServerIncludes) $(INCLUDES) -c $(src)/main.cpp -o $(objFile)/main.o ${option} 

$(objFile)/AirPlayerServer.o: 
	g++  $(INCLUDES) -c $(src)/AirPlayerServer.cpp -o $(objFile)/AirPlayerServer.o ${option} 
	
$(objFile)/Server.o: Server.cpp
	g++  $(INCLUDES) -c $(src)/Server.cpp -o $(objFile)/Server.o ${option} 
	
$(objFile)/Command.o:
	g++  $(INCLUDES) -c $(src)/Command.cpp -o $(objFile)/Command.o ${option} 
	
$(objFile)/LsCommand.o:
	g++  $(INCLUDES) -c $(src)/LsCommand.cpp -o $(objFile)/LsCommand.o ${option} 
	
$(objFile)/PlayCommand.o:
	g++ $(INCLUDES) -c $(src)/PlayCommand.cpp -o $(objFile)/PlayCommand.o ${option} 

$(objFile)/DescribeCommand.o:
	g++  $(INCLUDES) -c $(src)/DescribeCommand.cpp -o $(objFile)/DescribeCommand.o ${option} 
	
$(objFile)/CommandFactory.o:
	g++  $(INCLUDES) -c $(src)/CommandFactory.cpp -o $(objFile)/CommandFactory.o ${option}

$(objFile)/PreviewCommand.o:
	g++  $(INCLUDES) -c $(src)/PreviewCommand.cpp -o $(objFile)/PreviewCommand.o ${option}
	
clean:
	-rm $(objFile)/*.o
