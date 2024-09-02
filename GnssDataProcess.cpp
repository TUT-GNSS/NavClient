#include "GnssDataProcess.h"

GnssDataProcess::GnssDataProcess()
{
}

GnssDataProcess::~GnssDataProcess()
{
}

void GnssDataProcess::dataProcess(const std::string &buffer)
{
    m_gnssData.isReady = false;
    if (buffer.substr(0, 5) == "GNGGA")
    {
        handleGGA(buffer);
        m_gnssData.isReady = true;
    }
    else if (buffer.substr(0, 5) == "GNVTG")
    {
        // std::cout << "GNVTG" << std::endl;
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


void GnssDataProcess::handleGGA(const std::string &data)
{
    // 以","为分割将数据存入数据队列
    int old_idx = 0, cur_idx = data.find(',', old_idx + 1);
    while (cur_idx != std::string::npos)
    {
        m_dataQue.push(data.substr(old_idx + 1, cur_idx - old_idx - 1));
        old_idx = cur_idx;
        cur_idx = data.find(',', old_idx + 1);
    }

    // 根据idx判断数据类型
    int idx = 1;
    while (!m_dataQue.empty())
    {
        switch (idx)
        {
        case 2:
            m_gnssData.utcTime = m_dataQue.front();
            break;
        case 3:
            m_gnssData.latitude = m_dataQue.front();
            break;
        case 5:
            m_gnssData.lontitude = m_dataQue.front();
            break;
        case 8:
            m_gnssData.numSatVisit = m_dataQue.front();
            break;
        case 10:
            m_gnssData.msl = m_dataQue.front();
            break;
        default:
            break;
        }
        // 数据出队
        ++idx;
        m_dataQue.pop();
    }
}

void GnssDataProcess::handleVTG(const std::string &data)
{
    /*******待添加**********/
}

bool GnssDataProcess::isReady()
{
    return m_gnssData.isReady;
}
const std::string &GnssDataProcess::getReadyData()
{
    m_readyData.clear();
    m_readyData = m_gnssData.utcTime + "\t" + m_gnssData.latitude + "\t" + m_gnssData.lontitude + "\t" + m_gnssData.msl + "\t" + m_gnssData.numSatVisit;
    return m_readyData;
}