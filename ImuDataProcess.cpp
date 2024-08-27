#include "ImuDataProcess.h"

ImuDataProcess::ImuDataProcess()
{
    //初始化m_imuData大小
    m_imuData.ImuTime.resize(7);
    m_imuData.ImuAcc.resize(3);
    m_imuData.ImuGyro.resize(3);
    m_imuData.ImuAngle.resize(3);
}
ImuDataProcess::~ImuDataProcess()
{
}
// 提取时间数据，保存到m_imuData中
void ImuDataProcess::getImuTime(const std::vector<short> &Buffer)
{
    for (int i = 0; i < 6; ++i)
    {
        m_imuData.ImuTime[i] = std::to_string(Buffer[i + 2]);
    }
    m_imuData.ImuTime[6] = std::to_string((Buffer[8] << 8) | Buffer[9]);
}
void ImuDataProcess::getImuAcc(const std::vector<short> &Buffer)
{
    static const double k_acc = 16.0;
    for (int i = 0; i < m_imuData.ImuAcc.size(); ++i)
    {
        double accVal = (double)(Buffer[i + 3] << 8 | Buffer[i + 2]) / 32768.0 * k_acc;
        m_imuData.ImuAcc[i] = std::to_string(accVal >= k_acc ? accVal - 2.0 * k_acc : accVal);
    }
}
void ImuDataProcess::getImuGyro(const std::vector<short> &Buffer)
{
    static const double k_gyro = 2000.0;
    for (int i = 0; i < m_imuData.ImuGyro.size(); ++i)
    {
        double gyroVal = (double)(Buffer[i + 3] << 8 | Buffer[i + 2]) / 32768.0 * k_gyro;
        m_imuData.ImuGyro[i] = std::to_string(gyroVal >= k_gyro ? gyroVal - 2.0 * k_gyro : gyroVal);
    }
}
void ImuDataProcess::getImuAngle(const std::vector<short> &Buffer)
{
    static const double k_angle = 180.0;
    for (int i = 0; i < m_imuData.ImuAngle.size(); ++i)
    {
        double angleVal = (double)(Buffer[i + 3] << 8 | Buffer[i + 2]) / 32768.0 * k_angle;
        m_imuData.ImuAngle[i] = std::to_string(angleVal >= k_angle ? angleVal - 2.0 * k_angle : angleVal);
    }
}

void ImuDataProcess::dataProcess(const std::vector<short> &Buffer)
{
    std::string res; 
    switch (Buffer[1])
    {
    case TIME:
    {
        getImuTime(Buffer);
        break;
    }
    case ACC:
    {
        getImuAcc(Buffer);
        break;
    }
    case GYRO:
    {
        getImuGyro(Buffer);
        break;
    }
    case ANGLE:
    {
        getImuAngle(Buffer);
        std::cout << "输出！" << std::endl;
        break;
    }
    case READY:
    {
        // 发送m_ImuData数据
        std::string res;
        for (int i = 0; i < 3;++i){
            res += m_imuData.ImuAcc[i] + "\t";
        }
        for (int i = 0; i < 3; ++i)
        {
            res += m_imuData.ImuGyro[i] + "\t";
        }
        for (int i = 0; i < 3; ++i)
        {
            res += m_imuData.ImuAngle[i] + "\t";
        }
        std::cout << "读取" << std::endl;
        break;
    }
    default:
        break;
    }
}