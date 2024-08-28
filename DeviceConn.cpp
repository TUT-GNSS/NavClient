#include "DeviceConn.h"

DeviceConn::DeviceConn(DeciveType type,std::string port,u_int32_t baudrate,int timeout):m_type(type),m_port(port)
                                                ,m_baudrate(baudrate),m_gnssBufPtr(nullptr),m_imuBufPtr(nullptr)
{
    m_serial.setPort(m_port); // 设置串口设备
    m_serial.setBaudrate(m_baudrate);     // 设置波特率
    serial::Timeout to = serial::Timeout::simpleTimeout(timeout);
    m_serial.setTimeout(to);

    m_serial.open();//打开设备串口

    if(m_type==DeciveType::GNSS){ 
       m_gnssBufPtr=new GnssBufferProcess(m_serial); 
    }
    else if(m_type==DeciveType::IMU){
        m_imuBufPtr=new ImuBufferProcess(m_serial);
    }
}
DeviceConn::~DeviceConn()
{
    if(m_type==DeciveType::GNSS){ 
        delete(m_gnssBufPtr);
    }
    else if(m_type==DeciveType::IMU){
        delete(m_imuBufPtr);
    }
}
void DeviceConn::run(){
    if(m_gnssBufPtr){
      while(true){
        m_gnssBufPtr->handleBuffer();
      }
    }
    else if(m_imuBufPtr){
      while(true){
        m_imuBufPtr->handleBuffer();
      }
    }
}