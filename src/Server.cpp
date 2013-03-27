#include "Server.h"
#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <cstring>
#include <sys/stat.h>
#include <signal.h>
#include <dirent.h>
#include <net/if.h>
#include <unistd.h>


using namespace std;

//默认使用的网卡为eth0
extern char* ETH_NAME;

/*
 * 构造函数
 */
Server::Server(int portNum):listenfd(-1)
{
    char buf[HOST_NAME_MAX + 1];
    gethostname(buf, HOST_NAME_MAX);
    this->hostname = buf;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(portNum);
}

Server::~Server()
{
}

/*
 *获取本机的IPv4地址，返回值为ipv4地址或者主机名的字符串指针
 *失败返回NULL
 */
const char* Server::getLocalIP() 
{
	/*
	 *静态变量，只在第一次初始化并记录ipv4地址，以后访问该
	 *函数时直接返回结果。
	 */
	static string ip = "";
	if(ip != "")
		return ip.c_str();
		
    struct ifreq ifr;
    strncpy(ifr.ifr_name, ETH_NAME, IFNAMSIZ);
    ifr.ifr_name[IFNAMSIZ - 1] = 0;

    if(listenfd == -1){
        listenfd = socket(AF_INET, SOCK_STREAM, 0);
    }

    if(listenfd < 0){
        logInf(WARNING, "listen socket < 0");
        return NULL;
    }
    
    if(ioctl(listenfd, SIOCGIFADDR, &ifr) < 0)
    {
    	
        logInf(WARNING, "getLocalP: ioctl error");
        return this->hostname.c_str();
    }
    
    struct sockaddr_in addr;
    memcpy(&addr, &ifr.ifr_addr, sizeof(addr));
    ip = inet_ntoa(addr.sin_addr);
    return ip.c_str();
}

/*
 * 运行服务器
 */
void Server::run()
{
	this->init();
	this->serve(this->listenfd);
}

void Server::init()
{   
    this->listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(this->listenfd < 0)
        logInf(ERROR, "listen socket < 0");
        
    int reuse = 1;
    /*
     *采用地址服用初始化套接字端点，以防服务器终止并尝试立即重启时程序不工作。
     */
    if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
    	logInf(ERROR, "setsockopt error");

    if(bind(this->listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        logInf(ERROR, "bind error");

    if(listen(this->listenfd, 64) < 0)
        logInf(ERROR, "listen error");

    int port = getPort();
    const char* ip = getLocalIP();
    fprintf(stdout, "Server address: %s:%d\n", ip, port);
}

/*
 * 处理信息，并记录在log.txt中，出现错误时退出程序
 */
void Server::logInf(TYPE type, const string str)
{
	int fd = open("log.txt", O_CREAT| O_WRONLY | O_APPEND,0644);
	if(fd >= 0)
	{
        time_t t = time(NULL);
        struct tm *tmr = localtime(&t);
        char buf[BUFSIZE];
        memset(buf, 0, sizeof(buf));

        sprintf(buf, "%d-%d-%d %d:%d:%d ",\
                tmr->tm_year + 1900, \
                tmr->tm_mon + 1,tmr->tm_mday,tmr->tm_hour, \
                tmr->tm_min,tmr->tm_sec);

        int r;
        r = write(fd, buf, strlen(buf));
		r = write(fd, str.c_str(), str.size()); 
		r = write(fd,"\n",1);      
		r = close(fd);
		
		fprintf(stdout, "%s", buf);
		fprintf(stdout, "%s\n", str.c_str());
	}
	if(type == ERROR)
		exit(1);
}

/*
 *获取服务器监听的端口号
 */
int Server::getPort() const
{
    return ntohs(serv_addr.sin_port);
}

/*
 *设置服务器进听的端口号，要在run之前设置
 */
void Server::setPort(int portNum)
{
    serv_addr.sin_port = htons(portNum);
}

/*
 * 使进程成为守护进程
 */
void Server::daemonize(const string cmd)
{
    struct rlimit r1;
    
      
    //将文件模式创建屏蔽字设置为0
    umask(0);

    //获取文件描述符的最大数量
    if(getrlimit(RLIMIT_NOFILE, &r1) < 0)
    {
    	logInf(ERROR, cmd + ": can't get file limit.");
    }

    //调用fork，然后使父进程退出
    pid_t pid = fork();
    if(pid < 0)
    {
    	logInf(ERROR, cmd + ": can't fork.");
    }

    //调用setsid以创建一个新会话
    setsid();

    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if(sigaction(SIGHUP, &sa, NULL) < 0)
    {
    	logInf(ERROR, cmd + ": can't ignore SIGHUP");
    }
   
    //再次fork，并使父进程终止。保证守护进程不会是会话首进程以防止它
    //取得控制终端
    pid = fork();
    if(pid < 0)
    {
        logInf(ERROR, cmd + ": can't fork.");
    }
    else if(pid != 0)
        exit(1);

    //将当前工作目录更改为根目录
    if(chdir("/") < 0)
    {
    	logInf(ERROR, cmd + ": can't change directory to /");
    }

    //关闭不再需要的文件描述符
    if(r1.rlim_max == RLIM_INFINITY)
    {
        r1.rlim_max = 1024;
    }
    for(int i = 0; i < r1.rlim_max; i++)
    {
        close(i);
    }

    //打开/dev/null
    int fd0 = open("/dev/null", O_RDWR);
    int fd1 = dup(0);
    int fd2 = dup(0);

    openlog(cmd.c_str(), LOG_CONS, LOG_DAEMON);
    if(fd0 != 0 || fd1 != 1 || fd2 != 2)
    {
        syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
        exit(1);
    }
}

