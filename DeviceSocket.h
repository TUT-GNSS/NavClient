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

class DeviceSocket{
public:
    DeviceSocket(std::string addr,int port,std::string deciveID);
    ~DeviceSocket();
    ssize_t sendBuffer(void *data, size_t size);
    ssize_t recvBuffer(void *data);

private:
    int m_port;
    std::string m_addr;
    std::string m_deviceID;
    int m_socketfd;
};