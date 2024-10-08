#include "DeviceSocket.h"

DeviceSocket::DeviceSocket(std::string ip,int port,std::string deviceID):m_ip(ip),m_port(port),m_deviceID(deviceID)
{
    if ((m_socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("socket() failed.\n");
        exit(-1);
    }
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(m_port);
    servaddr.sin_addr.s_addr = inet_addr(m_ip.data());

    if (connect(m_socketfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("connect(%s:%d) failed.\n", m_ip.data(), m_port);
        close(m_socketfd);
        exit(-1);
    }
}
DeviceSocket::~DeviceSocket(){
    close(m_socketfd);
}
ssize_t DeviceSocket::sendBuffer(const std::string &data)
{
    size_t dataSize=data.size();
    char tmpbuf[1024]; // 临时的buffer，报文头部+报文内容。
    memset(tmpbuf, 0, sizeof(tmpbuf));
    memcpy(tmpbuf, &dataSize, 4);       // 拼接报文头部。
    memcpy(tmpbuf + 4, (void*)data.data(), dataSize); // 拼接报文内容。

    return send(m_socketfd, tmpbuf, dataSize + 4, 0); // 把请求报文发送给服务端。
}
ssize_t DeviceSocket::recvBuffer(void *data){
    int len;
    recv(m_socketfd, &len, 4, 0);          // 先读取4字节的报文头部。
    printf("recv len:%d",len);
    return recv(m_socketfd, data, len, 0); // 读取报文内容。
}

