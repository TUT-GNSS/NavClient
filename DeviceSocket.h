// 网络通讯类。
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
    //创建socketfd并连接服务器
    DeviceSocket(std::string ip,int port,std::string deviceID);
    ~DeviceSocket();
    //接收服务端发送的报文
    ssize_t recvBuffer(void *data);
    //客户端登录信息，向服务器发送deviceID登录报文
    void login();
    //Gnss报文发送
    ssize_t sendGnssBuffer(const std::string &data);
    //imu报文发送
    ssize_t sendImuBuffer(const std::string &data);
private:
    ssize_t sendBuffer(const std::string &data);

    std::string m_ip;//目的ip地址
    int m_port;//目的端口号
    std::string m_deviceID;//设备ID
    int m_socketfd;//客户端fd
};