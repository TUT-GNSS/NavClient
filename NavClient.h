#pragma once

#include <stdio.h>
#include <unistd.h>
#include "DeviceSerial.h"
#include "DeviceSocket.h"
#include "DeviceType.hpp"

//设备状态机
struct DeviceInfo{
    //socket初始化信息设置完毕
    void setSocketInfoIsReady()
    {
        socketInfoIsReady=true;
    }
    //gnss串口初始化信息设置完毕
    void setGnssInfoIsReady()
    {
        gnssInfoIsReady=true;
    }
    //imu串口初始化信息设置完毕
    void setImuInfoIsReady()
    {
        imuInfoIsReady=true;
    }
    //获取socket初始化信息配置是否完毕
    bool getSocketInfoState()
    {
        return socketInfoIsReady;
    }
    //获取gnss初始化信息配置是否完毕
    bool getGnssInfoState()
    {
        return gnssInfoIsReady;
    }
    //获取imu初始化信息配置是否完毕
    bool getImuInfoState()
    {
        return imuInfoIsReady;
    }

    //socket信息
    std::string ip;//服务端ip地址
    int socketPort;//服务端口号
    std::string deviceID="0";//客户端设备ID
    //Gnss设备信息
    static const DeviceType gnssType=DeviceType::GNSS;
    std::string gnssSerialPort = "/dev /ttyUSB1"; // 串口地址
    u_int32_t gnssBaudrate=9600;//波特率
    int gnssTimeout=1000;
    //Imu设备信息
    static const DeviceType imuType=DeviceType::IMU;
    std::string imuSerialPort = "/dev/ttyUSB0";
    u_int32_t imuBaudrate=115200;
    int imuTimeout=1000;
private:
    bool socketInfoIsReady=false; //socket信息是否设置完毕
    bool gnssInfoIsReady=false;
    bool imuInfoIsReady=false;

};

class NavClient{
public:
    NavClient(DeviceInfo deviceInfo);
    ~NavClient();
    //客户端登录信息，向服务器发送deviceID登录报文

    void run();//运行客户端程序
    void testSerial();//测试串口数据,打印串口数据信息

private:
    //向服务端发送登录请求
    void login();
    //Gnss报文发送
    ssize_t sendGnssBuffer(const std::string &data);
    //imu报文发送
    ssize_t sendImuBuffer(const std::string &data);

    DeviceInfo m_deviceInfo;//设备信息状态机
    DeviceSerial *m_gnssSerial;//Gnss串口处理类
    DeviceSerial *m_imuSerial;//Imu串口处理类
    DeviceSocket *m_deviceSocket;//设备Socket类
};

