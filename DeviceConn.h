#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "serial/serial.h"
#include "ImuBufferProcess.h"
#include "BufferProcess.h"
#include "GnssBufferProcess.h"
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
    void run();

private:
    std::string m_port;
    uint32_t m_baudrate;
    DeciveType m_type;
    serial::Serial m_serial;
    GnssBufferProcess *m_gnssBufPtr;
    ImuBufferProcess *m_imuBufPtr;
};