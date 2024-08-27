#pragma once
#include "ImuDataProcess.h"

class ImuBufferProcess{
public:
    ImuBufferProcess();
    ~ImuBufferProcess();
    void handleBuffer(const short inputData);

private:
    //处理inputData为协议头的函数
    void handleHeader();
    bool m_isStart = true;//是否为报文开始
    //校验和，数据当前索引，报文长度
    int m_checkSum = 0, m_dataIndex = 0, m_bufferLength = 11;
    std::vector<short> m_RxBuffer;
    ImuDataProcess m_imuDataprocess;
};