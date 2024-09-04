#include "DeviceSerial.h"

DeviceSerial::DeviceSerial(DeviceType type, std::string port, u_int32_t baudrate, int timeout) : m_type(type), m_port(port), m_baudrate(baudrate), m_bufferProcPtr(nullptr)
{
  m_serial.setPort(m_port);         // 设置串口设备
  m_serial.setBaudrate(m_baudrate); // 设置波特率
  serial::Timeout to = serial::Timeout::simpleTimeout(timeout);
  m_serial.setTimeout(to);

  std::cout << m_port << "   " << m_baudrate << std::endl;
  m_serial.open(); // 打开设备串口

  bufferProcFactory();
  if (m_bufferProcPtr)
  {
    m_bufferProcPtr->setReadCallback(std::bind(&DeviceSerial::readSerial, this, std::placeholders::_1)); // 绑定回调函数
  }
}
void DeviceSerial::bufferProcFactory()
{
  // 创建bufferProc的工厂类
  // 根据m_type实例化对应的报文处理成员对象
  switch (m_type)
  {
  case DeviceType::GNSS:
    m_bufferProcPtr = new GnssBufferProcess();
    break;
  case DeviceType::IMU:
    m_bufferProcPtr = new ImuBufferProcess();
    break;

  default:
    break;
  }
}

std::string DeviceSerial::readSerial(size_t sz)
{
  return m_serial.read(sz);
}

DeviceSerial::~DeviceSerial()
{
  // 释放bufferProc
  if (m_bufferProcPtr)
  {
    delete (m_bufferProcPtr);
  }
  m_serial.close(); // 关闭串口
}

const std::string &DeviceSerial::run()
{
  if (m_bufferProcPtr)
  {
    while (true)
    {
      m_bufferProcPtr->handleBuffer();
      if (m_bufferProcPtr->isReady())
      {
        return m_bufferProcPtr->getReadyBuffer();
      }
    }
  }
  sleep(1);//休眠一秒,防止发送过快。
  return NULLSTR;
}