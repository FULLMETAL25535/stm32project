#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sqlite3.h>
#include <signal.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DATABASE "my.db"
#define N  16

enum CMD{
    R=1,
    L,
    Q,
    H
};

typedef struct sockaddr SA;

typedef struct
{
    int type;
    char name[N];
    char data[256];   // password or word
} MSG;

static sqlite3 *db;

/**
 * @brief 服务器处理注册
 * 得到用户输入用户名和密码并拼接sql语句
 * 执行sql语句
 * 封装执行结果并回发客户端
 */
void do_register(int connfd, MSG *pbuf)
{

}

/**
 * @brief 服务器处理登录
 * 得到用户输入用户名和密码并拼接sql语句
 * 执行sql语句
 * 封装执行结果并回发客户端
 */
void do_login(int connfd, MSG *pbuf)
{

}

/**
 * @brief 查询单词释义
 * @return 查询成功返回1 失败返回0
 */
int do_searchword(int connfd, MSG *pbuf)
{
    
}

/**
 * @brief 返回当前的时间，存到date中
 * @return 
 */
void get_date(char *date)
{
    time_t t;
    struct tm *tp;

    time(&t);

    tp = localtime(&t);
    /*
    sprintf(date, "%d-%02d-%02d %02d:%02d:%02d", tp->tm_year+1900,
            tp->tm_mon+1, tp->tm_mday, tp->tm_hour, tp->tm_min, tp->tm_sec);
    */
    strftime(date, 64, "%Y-%m-%d %H:%M:%S", tp);

    return;
}

/**
 * @brief 服务器处理查询
 * 查询单词释义
 * 把本次查询添加到查询历史中
 */
void do_query(int connfd, MSG *pbuf)
{
   
}

/**
 * @brief 查询历史处理
 * 根据用户名来查找查询历史，并将结果返回给客户端
 * 执行sql语句
 * 封装执行结果并回发客户端
 */
void do_history(int connfd, MSG *pbuf)
{
    return;
}

/**
 * @brief do_client 处理客户端请求
 * @param connfd 客户端fd
 * @param db 数据库句柄
 */
void do_client(int connfd)
{
    MSG buf;
    while(recv(connfd, &buf, sizeof(buf), 0) > 0){
        switch (buf.type) {
        case R:
            printf("will reg\n");
            do_register(connfd, &buf);
            break;
        case L:
            printf("will login\n");
            do_login(connfd, &buf);
            break;
        case Q:
            printf("will query\n");
            do_query(connfd, &buf);
            break;
        case H:
            printf("will history\n");
            do_history(connfd, &buf);
            break;
        default:
            break;
        }
    }

    exit(0);
}


int main(int argc, char *argv[])
{
    int listenfd, connfd;
    struct sockaddr_in myaddr;
    pid_t pid;
    MSG buf;
    

    if (argc < 3)
    {
        printf("Usage : %s <ip> <port>\n", argv[0]);
        exit(-1);
    }

    //打开数据库
    if(sqlite3_open(DATABASE, &db) < 0){
        printf("fail to sqlite3_open : %s\n", sqlite3_errmsg(db));
        return -1;
    }

    //创建服务器socket
    listenfd = socket(PF_INET, SOCK_STREAM, 0);
    if(listenfd < 0){
        perror("fail to socket");
        exit(-1);
    }
    bzero(&myaddr, sizeof(myaddr));
    myaddr.sin_family = PF_INET;
    myaddr.sin_port = htons(atoi(argv[2]));
    myaddr.sin_addr.s_addr = inet_addr(argv[1]);
    if (bind(listenfd, (SA *)&myaddr, sizeof(myaddr)) < 0)
    {
        perror("fail to bind");
        exit(-1);
    }

    // XXX int listen(int sockfd, int backlog);
    if (listen(listenfd, 5) < 0)
    {
        perror("fail to listen");
        exit(-1);
    }

    //并发服务器模型
    while(1){
        if((connfd = accept(listenfd, NULL, NULL)) < 0){
            perror("fail to accept");
            exit(-1);
        }
        pid = fork();
        if(pid == -1){
            perror("fail to fork\n");
            exit(-1);
        }
        else if(pid == 0){ //子进程
            printf("a user comming\n");
            do_client(connfd);
        }else{  //父进程
            close(connfd);
        }
    }
}
