#include "GnssDataProcess.h"

GnssDataProcess::GnssDataProcess()
{
    m_slidingStr = "";
}

GnssDataProcess::~GnssDataProcess()
{
}

void GnssDataProcess::DataProcess(const std::string &in_data1)
{
    if (m_slidingStr.size() < 2)
    {
        m_slidingStr += in_data1;
    }
    m_slidingStr[0] = m_slidingStr[1];
    m_slidingStr[1] = in_data1[0];
    if (m_slidingStr == "GN")
    {
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

void GnssDataProcess::setGGACallback(std::function<std::string()> fn)
{
    m_getGGACallback = fn;
}
void GnssDataProcess::setVTGCallback(std::function<std::string()> fn)
{
    m_getVTGCallback = fn;
}

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