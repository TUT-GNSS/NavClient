#include "ImuBufferProcess.h"

ImuBufferProcess::ImuBufferProcess()
{
    m_RxBuffer.resize(m_bufferLength);
    m_imuDataprocess = ImuDataProcess();
}
ImuBufferProcess::~ImuBufferProcess()
{

}
void ImuBufferProcess::handleHeader()
{
    m_isStart = false;
    m_dataIndex = 0;
    m_checkSum = 0;
    std::fill(m_RxBuffer.begin(), m_RxBuffer.end(), 0);
}
void ImuBufferProcess::handleBuffer(const short inputData)
{
    //如果输入数据位协议头
    if(inputData==0x55&&m_isStart){
        handleHeader();
    }
    if(!m_isStart)
    {
        m_checkSum += inputData;//累加校验和
        m_RxBuffer[m_dataIndex++] = inputData; // 存入m_RxBuffer
        //报文读取完毕
        if (m_dataIndex == m_bufferLength)
        {
            //计算最终校验和
            m_checkSum = (m_checkSum - inputData) & 0xff;
            m_isStart = true;
            //判断报文校验和是否正确
            if (m_RxBuffer[m_bufferLength - 1] == m_checkSum)
            {
                //处理Imu数据
                m_imuDataprocess.dataProcess(m_RxBuffer);
            }
        }
    }
}