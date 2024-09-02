#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "serial/serial.h"
#include "ImuBufferProcess.h"
#include "BufferProcess.h"
#include "GnssBufferProcess.h"
#include "DeviceType.hpp"

// 定义无设备连接时返回的字符串
static const std::string NULLSTR = "NAN";

// 设备串口类，用于初始化串口设备、打开、读取指定大小数据。
class DeviceSerial
{
public:
    DeviceSerial(DeviceType type, std::string port, u_int32_t baudrate, int timeout);
    ~DeviceSerial();
    const std::string &run();

private:
    void bufferProcFactory(); // 创建bufferProc的工厂类
    std::string readSerial(size_t sz);
    std::string m_port;
    uint32_t m_baudrate;
    DeviceType m_type;
    serial::Serial m_serial;
    // GnssBufferProcess *m_gnssBufPtr;
    // ImuBufferProcess *m_imuBufPtr;
    BufferProcess *m_bufferProcPtr;
    BufferProcess *m_imuBufPtr;
};