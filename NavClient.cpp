#include "NavClient.h"

NavClient::NavClient(DeviceInfo deviceInfo):m_deviceInfo(deviceInfo)                       
{
    if(m_deviceInfo.getSocketInfoState()){
        m_deviceSocket = new DeviceSocket(m_deviceInfo.ip, m_deviceInfo.socketPort, m_deviceInfo.deviceID);
    }
    if(m_deviceInfo.getGnssInfoState()){
        m_gnssSerial=new DeviceSerial(m_deviceInfo.gnssType,m_deviceInfo.gnssSerialPort,m_deviceInfo.gnssBaudrate,m_deviceInfo.gnssTimeout);
    }
    if(m_deviceInfo.getImuInfoState()){
        m_imuSerial=new DeviceSerial(m_deviceInfo.imuType,m_deviceInfo.imuSerialPort,m_deviceInfo.imuBaudrate,m_deviceInfo.imuTimeout);
    }
}

NavClient::~NavClient()
{
    if(m_deviceInfo.getGnssInfoState()){
        delete(m_gnssSerial);
    }
    if(m_deviceInfo.getImuInfoState()){
        delete(m_imuSerial);
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
    login();
    // 创建一个子进程给gnss
    if (m_deviceInfo.getGnssInfoState() && m_deviceInfo.getSocketInfoState())
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork failed!");
            return;
        }
        else if (pid == 0)//子进程执行
        {
            while (true)
            {
                // std::cout << "GNSS Data:" << m_gnssSerial->run() << "\n";
                sendGnssBuffer(m_gnssSerial->run());
            }
        }
    }

    //创建一个子进程给imu
    if(m_deviceInfo.getImuInfoState()&& m_deviceInfo.getSocketInfoState()){
        pid_t pid=fork();
        if(pid==-1){
            perror("fork failed!");
            return;
        }
        else if (pid == 0) // 子进程执行
        {
            while(true){
                // std::cout << "IMU Data:" << m_imuSerial->run() << "\n";
                sendImuBuffer(m_imuSerial->run());
            }
        }
    }
    //父进程循环等待
    while(true){

    }
}