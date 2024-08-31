#include "DeviceConn.h"

DeviceConn::DeviceConn(DeviceType type,std::string port,u_int32_t baudrate,int timeout):m_type(type),m_port(port)
                                                ,m_baudrate(baudrate),m_gnssBufPtr(nullptr),m_imuBufPtr(nullptr)
{
    m_serial.setPort(m_port); // 设置串口设备
    m_serial.setBaudrate(m_baudrate);     // 设置波特率
    serial::Timeout to = serial::Timeout::simpleTimeout(timeout);
    m_serial.setTimeout(to);
    std::cout << m_port<<"   "<<m_baudrate << std::endl;
    m_serial.open(); // 打开设备串口

    if(m_type==DeviceType::GNSS){ 
       m_gnssBufPtr=new GnssBufferProcess(m_serial); 
    }
    else if(m_type==DeviceType::IMU){
        m_imuBufPtr=new ImuBufferProcess(m_serial);
    }
}
DeviceConn::~DeviceConn()
{
    if(m_type==DeviceType::GNSS){ 
        delete(m_gnssBufPtr);
    }
    else if(m_type==DeviceType::IMU){
        delete(m_imuBufPtr);
    }
    m_serial.close();
}

const std::string& DeviceConn::run(){
    if(m_gnssBufPtr){
      while(true){
        m_gnssBufPtr->handleBuffer();
        if(m_gnssBufPtr->isReady()){
          return m_gnssBufPtr->getReadyBuffer();
        }
      }
    }
    else if(m_imuBufPtr){
      while(true){
        m_imuBufPtr->handleBuffer();
        if(m_imuBufPtr->isReady()){
          return m_imuBufPtr->getReadyBuffer();
        }
      }
    }
    return {};
}