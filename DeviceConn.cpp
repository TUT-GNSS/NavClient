#include "DeviceConn.h"

DeviceConn::DeviceConn(DeciveType type,std::string port,u_int32_t baudrate,int timeout)
{
    m_serial.setPort(port); // 设置串口设备
    m_serial.setBaudrate(115200);     // 设置波特率
    serial::Timeout to = serial::Timeout::simpleTimeout(timeout);
    m_serial.setTimeout(to);
}
DeviceConn::~DeviceConn()
{

}
void DeviceConn::open()
{

}