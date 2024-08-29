#pragma once
#include "DeviceConn.h"
#include "DeviceSocket.h"
#include "DeviceType.hpp"

//设备状态机
struct DeviceInfo{
    //socket信息
    std::string ip;
    int socketPort;
    std::string deviceID="0";
    //Gnss设备信息
    static const DeviceType gnssType=DeviceType::GNSS;
    std::string gnssSerialPort;
    u_int32_t gnssBaudrate=9600;
    int gnssTimeout=1000;
    //Imu设备信息
    static const DeviceType imuType=DeviceType::IMU;
    std::string imuSerialPort;
    u_int32_t imuBaudrate=115200;
    int imuTimeout=1000;
};

class NavClient{
public:
    NavClient(DeviceInfo deviceInfo);
    ~NavClient();

private:
    DeviceInfo m_deviceInfo;
    DeviceConn *m_gnssConn;
    DeviceConn *m_imuConn;
    DeviceSocket *m_deviceSocket;
};