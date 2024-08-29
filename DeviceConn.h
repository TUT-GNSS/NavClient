#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "serial/serial.h"
#include "ImuBufferProcess.h"
#include "BufferProcess.h"
#include "GnssBufferProcess.h"
#include "DeviceType.hpp"
// enum class DeviceType
// {
//     IMU,
//     GNSS
// };

//设备连接类，用于初始化串口设备、打开、读取指定大小数据。
class DeviceConn{
public:
    DeviceConn(DeviceType type,std::string port,u_int32_t baudrate,int timeout);
    ~DeviceConn();
    const std::string& run();

private:
    std::string m_port;
    uint32_t m_baudrate;
    DeviceType m_type;
    serial::Serial m_serial;
    GnssBufferProcess *m_gnssBufPtr;
    ImuBufferProcess *m_imuBufPtr;
};