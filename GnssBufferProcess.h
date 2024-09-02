#pragma once
#include <iostream>
#include <string>
#include <functional>
#include "GnssDataProcess.h"
#include "serial/serial.h"
#include "BufferProcess.h"

class GnssBufferProcess:public BufferProcess
{
public:    
    GnssBufferProcess();
    ~GnssBufferProcess();
    void handleBuffer() override;//Buffer处理
    bool isReady() override;//处理的报文是否ready
    const std::string &getReadyBuffer() override;//得到isReady的Buffer

private:
    std::string m_buffer;
    int m_checkSum = 0;//校验和******暂未添加
    GnssDataProcess m_gnssDataProcess;
};