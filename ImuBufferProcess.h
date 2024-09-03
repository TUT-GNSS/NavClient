#pragma once
#include "serial/serial.h"
#include "ImuDataProcess.h"
#include "BufferProcess.h"

class ImuBufferProcess:public BufferProcess{
public:
    ImuBufferProcess();
    ~ImuBufferProcess();
    void handleBuffer() override;//Buffer处理
    bool isReady() override;//处理的报文是否ready
    const std::string &getReadyBuffer() override; // 得到isReady的Buffer

private:
    void handleHeader();//处理报文协议头
    bool m_isStart = true;//是否为报文开始
    //校验和，数据当前索引，报文长度
    int m_checkSum = 0, m_dataIndex = 0, m_bufferLength = 11;
    std::vector<short> m_RxBuffer;//存放报文数据的数组
    ImuDataProcess m_imuDataprocess;
};