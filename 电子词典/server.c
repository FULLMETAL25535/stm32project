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

void do_register(int connfd, MSG *pbuf)
{
    char sqlstr[128];
    char *errmsg;

    sprintf(sqlstr, "insert into usr values ('%s', '%s')", pbuf->name, pbuf->data);
    printf("%s\n", sqlstr);
    if (sqlite3_exec(db, sqlstr, NULL, NULL, &errmsg) != SQLITE_OK)
    {
        sqlite3_free(errmsg);
        sprintf(pbuf->data, "user %s already exist!!!", pbuf->name);
    }
    else
    {
        strncpy(pbuf->data, "OK", 256);
    }
    send(connfd, pbuf, sizeof(MSG), 0);
}


void do_login(int connfd, MSG *pbuf)
{
    char sqlstr[128];
    char *errmsg, **result;
    int nrow, ncolumn;

    sprintf(sqlstr, "select * from usr where name = '%s' and pass = '%s'", pbuf->name, pbuf->data);
    if (sqlite3_get_table(db, sqlstr, &result, &nrow, &ncolumn, &errmsg) != SQLITE_OK)
    {
        printf("error : %s\n", errmsg);
        sqlite3_free(errmsg);
    }

    if (nrow == 0)
    {
        strncpy(pbuf->data, "name or password is wrong!!!", 256);
    }
    else
    {
        strncpy(pbuf->data, "OK", 256);
    }
    send(connfd, pbuf, sizeof(MSG), 0);
    sqlite3_free_table(result);

    return;
}

#if 0
int do_searchword(int connfd, MSG *pbuf)
{
    FILE *fp;
    char line[300];
    char *p;
    int len, result;

    len = strlen(pbuf->data);
    if ((fp = fopen("dict.txt", "r")) == NULL)
    {
        strcpy(pbuf->data, "dict on server can't be opened :(");
        send(connfd, pbuf, sizeof(MSG), 0);
        return 0;
    }
    printf("query word is %s\n", pbuf->data);
    while (fgets(line, 300, fp) != NULL)
    {
        result = strncmp(pbuf->data, line, len);
        if (result > 0) continue;
        if (result < 0 || line[len] != ' ') break;

        p = line + len;
        while (*p == ' ') p++;
        strcpy(pbuf->data, p);
        fclose(fp);
        return 1;
    }
    fclose(fp);

    return 0;
}
#else
int do_searchword(int connfd, MSG *pbuf)
{
    char buf[128] = {0};

    printf("query word is %s\n", pbuf->data);
    sprintf(buf, "select * from dic where word='%s'", pbuf->data);

    char *errmsg;
	char **resultp;
	int n_row, n_cloum;

	if (sqlite3_get_table(db, buf, &resultp, &n_row, &n_cloum, &errmsg) != SQLITE_OK) 
    {
        printf("err:%s\n", errmsg);
        return -1;
    }

    if(n_row != 1)
    {
        return 0;
    }

    char **p;
    p = resultp + n_cloum;

    // printf("data=%s\n", p[1]);

    strcpy(pbuf->data, p[1]);

    return 1;
}
#endif

void get_date(char date[])
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

void do_query(int connfd, MSG *pbuf)
{
    char sqlstr[128], *errmsg;
    int len, result, found = 0;
    char date[64], word[64];

    strcpy(word, pbuf->data);
    found = do_searchword(connfd, pbuf);
    if(found)
    {
        get_date(date);
        sprintf(sqlstr, "insert into record values ('%s', '%s', '%s')", pbuf->name, date, word);
        if (sqlite3_exec(db, sqlstr, NULL, NULL, &errmsg) != SQLITE_OK)
        {
            printf("error : %s\n", errmsg);
            sqlite3_free(errmsg);
        }
    }
    else
    {
        strcpy(pbuf->data, "not found\n");
    }
    send(connfd, pbuf, sizeof(MSG), 0);
    return;
}

int history_callback(void *arg, int f_num, char **f_value, char **f_name)
{
    int connfd;
    MSG buf;

    connfd = *(int *)arg;
    sprintf(buf.data, "%s : %s", f_value[1], f_value[2]);
    send(connfd, &buf, sizeof(buf), 0);

    return 0;
}

void do_history(int connfd, MSG *pbuf)
{
    char sqlstr[128], *errmsg;

    sprintf(sqlstr, "select * from record where name = '%s'", pbuf->name);
    if (sqlite3_exec(db, sqlstr, history_callback, (void *)&connfd, &errmsg) != SQLITE_OK)
    {
        printf("error : %s\n", errmsg);
        sqlite3_free(errmsg);
    }
    pbuf->data[0] = '\0';
    send(connfd, pbuf, sizeof(MSG), 0);

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
