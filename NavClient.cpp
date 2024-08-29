#include "NavClient.h"

NavClient::NavClient(DeviceInfo deviceInfo):m_deviceInfo(deviceInfo)                       
{
    m_deviceSocket=new DeviceSocket(m_deviceInfo.ip,m_deviceInfo.socketPort,m_deviceInfo.deviceID);
    if(m_deviceInfo.getGnssInfoState()){
        m_gnssConn=new DeviceConn(m_deviceInfo.gnssType,m_deviceInfo.gnssSerialPort,m_deviceInfo.gnssBaudrate,m_deviceInfo.gnssTimeout);
    }
    if(m_deviceInfo.getImuInfoState()){
        m_imuConn=new DeviceConn(m_deviceInfo.imuType,m_deviceInfo.imuSerialPort,m_deviceInfo.imuBaudrate,m_deviceInfo.imuTimeout);
    }
}

NavClient::~NavClient()
{
    if(m_deviceInfo.getGnssInfoState()){
        delete(m_gnssConn);
    }
    if(m_deviceInfo.getImuInfoState()){
        delete(m_imuConn);
    }
    if(m_deviceInfo.getSocketInfoState()){
        delete(m_deviceSocket);
    }
}

void NavClient::login()
{
    m_deviceSocket->sendBuffer("<bizcode>00101</bizcode><deviceid>"+m_deviceInfo.deviceID+"</deviceid>");
}
ssize_t NavClient::sendGnssBuffer(const std::string &data)
{
    return m_deviceSocket->sendBuffer("<bizcode>00201</bizcode><gnssdata>"+data+"</gnssdata>");
}
ssize_t NavClient::sendImuBuffer(const std::string &data)
{
    return m_deviceSocket->sendBuffer("<bizcode>00202</bizcode><imudata>"+data+"</imudata>");
}

void NavClient::run()
{
    while(1){
        std::cout<<m_imuConn->run()<<"\n";
    }
}