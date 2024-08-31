#include "GnssDataProcess.h"

GnssDataProcess::GnssDataProcess()
{
    // m_slidingStr = "";
}

GnssDataProcess::~GnssDataProcess()
{
}

void GnssDataProcess::dataProcess(const std::string &buffer)
{
    if(buffer.substr(0,5)=="GNGGA"){
        // handleGGA(buffer.substr(6));
        std::cout << buffer << std::endl;
    }
    else if(buffer.substr(0,5)=="GNVTG"){
        // handleGGA(buffer.substr(6));
        std::cout << buffer << std::endl;
    }
    // if (m_slidingStr.size() < 5)
    // {
    //     m_slidingStr += inputData;
    // }
    // else{  
    //     //滑动字符串，后续需要优化
    //     m_slidingStr.erase(0,1);
    //     m_slidingStr+=inputData;
    //     m_gnssData.isReady = false;

    //     if (m_slidingStr == "GNGGA")
    //     {
    //         std::cout << "GGA:      " << m_getGGABufferCallback() << std::endl;
    //         // handleGGA(m_getGGABufferCallback());
    //         // m_gnssData.isReady = true;
    //     }
    //     else if(m_slidingStr=="GNVTG"){
    //         std::cout<<"VTG:      " << m_getVTGBufferCallback() << std::endl;
    //         // handleVTG(m_getVTGBufferCallback());
    //         // m_gnssData.isReady = true;
    //     }
    // }
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
    std::regex re(R"(\w+)(?:,|$)");
    std::sregex_token_iterator iter(data.begin(), data.end(), re, 0);
    std::vector<std::string> tokens(iter, {});

    if (tokens.size() < 13)
    {
        std::cout << "GPS not found\n";
        m_gnssData.gps_t = false;
        return;
    }

    m_gnssData.utcTime = tokens[0];
    m_gnssData.latitude = std::to_string(degreesConvert(tokens[2], tokens[3]));
    m_gnssData.lontitude = std::to_string(degreesConvert(tokens[5], tokens[6]));
    m_gnssData.numSatVisit = tokens[9];
    m_gnssData.msl = tokens[12] + "." + tokens[13] + tokens[14];
    m_gnssData.gps_t = true;
}

void GnssDataProcess::handleVTG(const std::string &data)
{
    std::regex re(R"(\w+)(?:,|$)");
    std::sregex_token_iterator iter(data.begin(), data.end(), re, 0);
    std::vector<std::string> tokens(iter, {});

    if (tokens.size() < 4 || !m_gnssData.gps_t)
        return;
    m_gnssData.cogt = tokens[0] + "." + tokens[1] + 'T';
    if (tokens[3] == "M")
    {
        m_gnssData.cogm = "0.00";
        m_gnssData.sog = tokens[4] + "." + tokens[5];
    }
    else
    {
        m_gnssData.cogm = tokens[3] + "." + tokens[4];
        m_gnssData.sog = tokens[6] + "." + tokens[7];
    }
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