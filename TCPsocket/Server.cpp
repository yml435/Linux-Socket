#include"Server.h"
void wait_childDead(int signal){
    pid_t pid;
    int stat;
    while((pid=waitpid(0,&stat,WNOHANG))>0);
        printf("wait for pid %d\n termained",pid);
    return;
}
void str_echo(int sockfd){
    size_t n;
    char buf[MAXLINE];
again:
    while((n=read(sockfd,buf,MAXLINE))>0){
        std::cout<<buf;
        Writen(sockfd,buf,n);
        memset(buf,0,n+10);
        if(n<0&&errno==EINTR)
            goto again;
        else if(n<0){
            err_sys("error for write");
        }
    }
}
void server_pig2_5(){
    int listenfd,connfd;
    pid_t pid;
    socklen_t clilen;
    struct sockaddr_in cliaddr;
    struct sockaddr_in servaddr;
    listenfd=Socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(9877);
    Bind(listenfd,(SA*)&servaddr,sizeof(servaddr));
    Listen(listenfd,5);
    if(signal(SIGCHLD,wait_childDead)==SIG_ERR){
        std::cout<<"signal process error"<<std::endl;
    }
    while(1){
        //std::cout<<"test"<<std::endl;
        clilen=sizeof(cliaddr);
        connfd=Accept(listenfd,(SA*)&cliaddr,&clilen);
        if((pid=fork())==0){
            close(listenfd);
            str_echo(connfd);
            exit(0);
        }
        Close(connfd);
    }
}
