#pragma once
#include <functional>
#include "serial/serial.h"

class BufferProcess
{
public:
    BufferProcess();
    ~BufferProcess();
    virtual void handleBuffer() = 0;//处理报文的纯虚函数，子类重写该方法
    virtual bool isReady() = 0;     // 发送报文是否准备好的纯虚函数，子类重写该方法
    virtual const std::string &getReadyBuffer() = 0;// 获取发送报文的纯虚函数，子类重写该方法
    void setReadCallback(std::function<std::string(size_t)> fn); // 设置回调函数

protected:
    std::function<std::string(size_t)> m_serialReadCallback; // 从串口读取指定大小数据的回调函数
    std::string readSerial(size_t sz);                       // 从串口读取指定大小数据
};