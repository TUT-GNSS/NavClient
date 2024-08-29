#include "NavClient.h"

NavClient::NavClient(DeviceInfo deviceInfo):m_deviceInfo(deviceInfo)                       
{
    m_gnssConn=new DeviceConn(m_deviceInfo.gnssType,m_deviceInfo.gnssSerialPort,m_deviceInfo.gnssBaudrate,m_deviceInfo.gnssTimeout);
    m_imuConn=new DeviceConn(m_deviceInfo.imuType,m_deviceInfo.imuSerialPort,m_deviceInfo.imuBaudrate,m_deviceInfo.imuTimeout);
    m_deviceSocket=new DeviceSocket(m_deviceInfo.ip,m_deviceInfo.socketPort,m_deviceInfo.deviceID);
}

NavClient::~NavClient()
{
    delete(m_gnssConn);
    delete(m_imuConn);
    delete(m_deviceSocket);
}