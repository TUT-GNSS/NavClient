#include "GnssDataProcess.h"

GnssDataProcess::GnssDataProcess()
{
}

GnssDataProcess::~GnssDataProcess()
{
}

void GnssDataProcess::dataProcess(const std::string &buffer)
{
    if(buffer.substr(0,5)=="GNGGA"){

        std::cout << "GNGGA" << std::endl;
    }
    else if(buffer.substr(0,5)=="GNVTG"){

        std::cout << "GNVTG" << std::endl;
    }

}

double GnssDataProcess::degreesConvert(const std::string &in_data1, const std::string &in_data2)
{
    // 转换in_data2为整数，并格式化为5位字符串，不足前面补0
    std::string str_data2 = std::to_string(std::stoi(in_data2));
    while (str_data2.length() < 5)
    {
        str_data2 = '0' + str_data2;
    }
    // 转换in_data1为整数，并处理符号
    int temp_data = std::stoi(in_data1);
    int symbol = 1;
    if (temp_data < 0)
    {
        symbol = -1;
    }
    // 提取度数
    int degree = temp_data / 100;
    // 构建小数部分字符串
    std::string str_decimal = in_data1.substr(in_data1.length() - 2, 2) + str_data2;
    // 转换小数部分字符串为double并计算
    double f_degree = std::stod(str_decimal) / 60.0 / 100000.0;
    // 根据符号计算最终结果
    double result = (symbol > 0) ? (degree + f_degree) : (degree - f_degree);

    return result;
}
// //设置读取GGA回调函数
// void GnssDataProcess::setGetGGABufferCallback(std::function<std::string()> fn)
// {
//     m_getGGABufferCallback = fn;
// }
// void GnssDataProcess::setGetVTGBufferCallback(std::function<std::string()> fn)
// {
//     m_getVTGBufferCallback = fn;
// }

void GnssDataProcess::handleGGA(const std::string &data)
{

}

void GnssDataProcess::handleVTG(const std::string &data)
{

}

bool GnssDataProcess::isReady()
{
    return m_gnssData.isReady;
}
const std::string &GnssDataProcess::getReadyData()
{
    m_readyData.clear();
    m_readyData = m_gnssData.utcTime + "\t" + m_gnssData.latitude + "\t" + m_gnssData.lontitude + "\t" + m_gnssData.msl + "\t" + m_gnssData.numSatVisit + "\t" + m_gnssData.cogt + "\t" + m_gnssData.cogm + "\t" + m_gnssData.sog;
    return m_readyData;
}