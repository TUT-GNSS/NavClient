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
    std::string readSerial(size_t sz); // 从串口读sz的数据并返回string，在BufferProcess::readSerial中回调此函数
    std::string m_port;//设备串口地址号
    uint32_t m_baudrate;//设备波特率
    DeviceType m_type;//设备类型
    serial::Serial m_serial;//设备对象
    BufferProcess *m_bufferProcPtr;//报文处理成员的指针，根据m_type类型实例化具体对象
};