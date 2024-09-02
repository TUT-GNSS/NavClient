#pragma once
#include <functional>
#include "serial/serial.h"

class BufferProcess
{
public:
    BufferProcess();
    ~BufferProcess();
    virtual void handleBuffer() = 0;
    virtual bool isReady() = 0;
    virtual const std::string &getReadyBuffer() = 0;
    void setReadCallback(std::function<std::string(size_t)> fn); // 设置回调函数

protected:
    std::function<std::string(size_t)> m_serialReadCallback; // 从串口读取指定大小数据的回调函数
    std::string readSerial(size_t sz);
};