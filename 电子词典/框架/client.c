#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sqlite3.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define N  16
typedef struct sockaddr SA;

enum CMD{
    R=1,  //注册
    L,  //登录
    Q,  //查询
    H  //历史
};


typedef struct
{
    int type;
    char name[N];
    char data[256];   // password or word
} MSG;

/**
 * @brief 客户端注册
 * 用户输入用户名和密码
 * 封装消息
 * 发送
 * 接收并输出结果
 */
void do_register(int sockfd, MSG *pbuf)
{

}

/**
 * @brief 客户端登录
 * 用户输入用户名和密码
 * 封装消息
 * 发送
 * 接收并输出结果
 * @return 成功返回1 失败返回0
 */
int do_login(int sockfd, MSG *pbuf)
{

}

/**
 * @brief 客户端查询
 * 查询历史
 */
void do_history(int sockfd, MSG *pbuf)
{

}

/**
 * @brief 客户端查询
 * 输入单词或“#”
 * 封装消息
 * 发送
 * 接收并输出结果
 */
void do_query(int sockfd, MSG *pbuf)
{

}

void enter_query(int sockfd, MSG *buf)
{
    int input;
    char cleanbuf[64];

    while (1) {
        printf("***********************************************\n");
        printf("* 1: query_word   2: history_record   3: quit *\n");
        printf("***********************************************\n");
        printf("please choose : ");
        //获取用户输入
        if(scanf("%d", &input) == 0){
            fgets(cleanbuf, 64, stdin);  //类型错误需要重新清除输入缓冲区
            continue;
        }

        switch (input) {
        case 1:
            printf("\n");
            do_query(sockfd, buf);
            printf("\n");
            break;
        case 2:
            printf("\n");
            do_history(sockfd, buf);
            printf("\n");
            break;
        case 3:
            return;
        default:
            break;
        }
    }
}

int main(int argc, char *argv[])
{
	int sockfd, login = 0;
	struct sockaddr_in servaddr;
	MSG buf;
	char clean[64];

	if (argc < 3)
	{
		printf("Usage : %s <serv_ip> <serv_port>\n", argv[0]);
		exit(-1);
	}

    //创建客户端socket
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("fail to socket");
		exit(-1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = PF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);

    //连接服务器
	if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("fail to connect");
		exit(-1);
	}

    int input;
    char cleanbuf[64];
    while(1){
        printf("************************************\n");
        printf("* 1: register   2: login   3: quit *\n");
        printf("************************************\n");
        printf("please choose : ");
        //获取用户输入
        if(scanf("%d", &input) == 0){
            fgets(cleanbuf, 64, stdin);  //类型错误需要重新清除输入缓冲区
            continue;
        }

        switch (input) {
        case 1:
            do_register(sockfd, &buf);
            break;
        case 2:
            if(do_login(sockfd, &buf) == 1){  //成功
                enter_query(sockfd, &buf);
            }
            break;
        case 3:
            close(sockfd);
            exit(0);
            break;
        default:
            break;
        }
    }
}
