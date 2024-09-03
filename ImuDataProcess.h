#pragma once
#include<iostream>
#include<string>
#include<vector>


//存放imu数据的结构体
struct ImuData
{
    std::vector<std::string> ImuTime; //"{yy}.{mm}.{dd} {hh}:{mn}:{ss}.{ms}"
    std::vector<std::string> ImuAcc;// AccX,AccY,AccZ
    std::vector<std::string> ImuGyro;//Wx,Wy,Wz
    std::vector<std::string> ImuAngle;//Roll,Pitch,Yaw
    bool isReady=false;
};

class ImuDataProcess{

public:
    ImuDataProcess();
    ~ImuDataProcess();
    // 数据提取，根据报文类型提取数据，并发送
    void dataProcess(const std::vector<short> &Buffer);
    bool isReady();
    const std::string & getReadyData();


private:
    // 提取时间数据，保存到m_imuData中
    void getImuTime(const std::vector<short> &Buffer);
    // 提取加速度数据，保存到m_imuData中
    void getImuAcc(const std::vector<short> &Buffer);
    // 提取角速度数据，保存到m_imuData中
    void getImuGyro(const std::vector<short> &Buffer);
    // 提取姿态角数据，保存到m_imuData中
    void getImuAngle(const std::vector<short> &Buffer);

    ImuData m_imuData;
    std::string m_readyData;
    static const short TIME = 0x50, ACC = 0x51, GYRO = 0x52, ANGLE = 0x53, READY = 0x5F;
};