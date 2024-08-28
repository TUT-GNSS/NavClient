// 网络通讯的客户端程序。
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <iostream>
#include <string>

ssize_t tcpsend(int fd, void *data, size_t size)
{
    char tmpbuf[1024]; // 临时的buffer，报文头部+报文内容。
    memset(tmpbuf, 0, sizeof(tmpbuf));
    memcpy(tmpbuf, &size, 4);       // 拼接报文头部。
    memcpy(tmpbuf + 4, data, size); // 拼接报文内容。

    return send(fd, tmpbuf, size + 4, 0); // 把请求报文发送给服务端。
}

ssize_t tcprecv(int fd, void *data)
{
    int len;
    recv(fd, &len, 4, 0);          // 先读取4字节的报文头部。
    printf("recv len:%d",len);
    return recv(fd, data, len, 0); // 读取报文内容。
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("usage:./client ip port\n");
        printf("example:./navClient 192.168.205.129 5005 001\n\n");
        return -1;
    }

    int sockfd;
    struct sockaddr_in servaddr;
    char buf[1024];
    std::string deviceID;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("socket() failed.\n");
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    deviceID = std::to_string(atoi(argv[3]));

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("connect(%s:%s) failed.\n", argv[1], argv[2]);
        close(sockfd);
        return -1;
    }

    printf("connect ok.\n");

    //////////////////////////////////////////
    // 登录业务。
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "<bizcode>00101</bizcode><deviceid>%s</deviceid>", deviceID.data());
    if (tcpsend(sockfd, buf, strlen((buf))) <= 0)
    {
        printf("tcpsend() failed.\n");
        return -1;
    }
    printf("发送：%s\n", buf);

    memset(buf, 0, sizeof(buf));
    if (tcprecv(sockfd, buf) <= 0)
    {
        printf("tcprecv() failed.\n");
        return -1;
    }
    printf("接收：%s\n", buf);
    //////////////////////////////////////////

    // //////////////////////////////////////////
    printf("开始时间:%ld\n", time(0));
    for (int i = 0; i < 10;++i){
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "<bizcode>00201</bizcode><navdata>19293 39310 3940</navdata>");
        if (tcpsend(sockfd, buf, strlen((buf))) <= 0)
        {
            printf("tcpsend() failed.\n");
            return -1;
        }
        printf("发送：%s\n", buf);

        memset(buf, 0, sizeof(buf));
        if (tcprecv(sockfd, buf) <= 0)
        {
            printf("tcprecv() failed.\n");
            return -1;
        }
        printf("接收：%s\n", buf);
    }
    printf("结束时间:%ld\n", time(0));
    //////////////////////////////////////////

    //////////////////////////////////////////
    // // 心跳。
    // while (true)
    // {
    //     memset(buf, 0, sizeof(buf));
    //     sprintf(buf, "<bizcode>00001</bizcode>");
    //     if (tcpsend(sockfd, buf, strlen((buf))) <= 0)
    //     {
    //         printf("tcpsend() failed.\n");
    //         return -1;
    //     }
    //     printf("发送：%s\n", buf);

    //     memset(buf, 0, sizeof(buf));
    //     if (tcprecv(sockfd, buf) <= 0)
    //     {
    //         printf("tcprecv() failed.\n");
    //         return -1;
    //     }
    //     printf("接收：%s\n", buf);

    //     // sleep(10);
    // }
    //////////////////////////////////////////

    // //////////////////////////////////////////
    // // 注销业务。
    // memset(buf, 0, sizeof(buf));
    // sprintf(buf, "<bizcode>00901</bizcode>");
    // if (tcpsend(sockfd, buf, strlen((buf))) <= 0)
    // {
    //     printf("tcpsend() failed.\n");
    //     return -1;
    // }
    // printf("发送：%s\n", buf);

    // memset(buf, 0, sizeof(buf));
    // if (tcprecv(sockfd, buf) <= 0)
    // {
    //     printf("tcprecv() failed.\n");
    //     return -1;
    // }
    // printf("接收：%s\n", buf);
    // //////////////////////////////////////////
    sleep(1);
}