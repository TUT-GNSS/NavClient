#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "serial/serial.h"
#include "ImuDataProcess.h"
enum class DeciveType
{
    IMU,
    GNSS
};

//设备连接类，用于初始化串口设备、打开、读取指定大小数据。
class DeviceConn{
public:
    DeviceConn(DeciveType type,std::string port,u_int32_t baudrate,int timeout);
    ~DeviceConn();
    void open();

private : 
    serial::Serial m_serial;
};