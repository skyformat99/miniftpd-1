#include "common.h"
#include "session.h"
#include "sysutil.h"
#include "str.h"
#include "parseconf.h"
#include "tunable.h"


int main(int argc,char *argv[])
{
	if( getuid() != 0 )
	{
		printf("run miniftpd must be root\n");
		exit(EXIT_FAILURE);
	}
	// test for str.h
	/* test	str_all_space
	char *str1 = "            a         e";
	char *str2 = "                    ";
	if( str_all_space(str1) )
	{
		printf("str1 is all space\n");
	}
	else
	{
		printf("str1 is not all space\n");
	}

	if( str_all_space(str2) )
	{
		printf("str2 is all space\n");
	}
	else
	{
		printf("str2 is not all space\n");
	}
	*/

	/* test str_upper
	char str3[] = "testTest";
	str_upper(str3);
	printf("str3:%s\n", str3);
	*/

	/* test str_to_longlong
	long long result = str_to_longlong("1234567890123");
	printf("%lld\n", result);
	*/

	/* test str_octal_to_uint
	unsigned int result = str_octal_to_uint("0755");
	printf("%d\n", result);
	*/

	// test for parseconf
	/*
	parseconf_load_file("miniftpd.conf");
	printf("%d\n", tunable_pasv_enable);
	printf("%d\n", tunable_port_enable);
	printf("%d\n", tunable_listen_port);
	printf("%d\n", tunable_max_clients);
	printf("%d\n", tunable_max_per_ip);
	printf("%d\n", tunable_accept_timeout);
	printf("%d\n", tunable_connect_timeout);
	printf("%d\n", tunable_idle_session_timeout);
	printf("%d\n", tunable_data_connection_timeout);
	printf("%o\n", tunable_local_umask);
	printf("%d\n", tunable_upload_max_rate);
	printf("%d\n", tunable_download_max_rate);		
	*/
	
	int listenfd = tcp_server(NULL,8888);
	
	session_t sess = {-1,"","","",-1,-1};
	pid_t pid;
	for( ; ; )
	{
		// 时间设置为0,阻塞接收连接
		int connfd = accept_timeout(listenfd,NULL,0);
		if( connfd == -1 )
			continue;
		// 创建子进程
		pid = fork();
		switch(pid)
		{
			case 0:
				// 子进程关闭listenfd，避免出现“惊群效应”
				close(listenfd);
				sess.ctrl_fd = connfd;
				begin_session(&sess);
				break;
			case -1:
				ERR_EXIT("fork");
				break;
			default:
				// 父进程关闭connfd
				close(connfd);
				break;
		}
	}

	return EXIT_SUCCESS;
}
