#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <functional>
#include<queue>

struct GnssData{
    std::string utcTime = "";     // #UTC时间
    std::string latitude = "";    // #latitude
    std::string lontitude = "";   // #longitude
    std::string numSatVisit = ""; // #可见卫星数
    std::string msl = "";  // #海拔高度Altitude，单位米（m）
    std::string cogt = ""; // #True north heading，单位degree  得到值+'T'+'°‘
    std::string cogm = ""; // #Magnetic north heading，单位degree（'°‘）
    std::string sog = ""; // #Ground speed，单位Kn
    bool gps_t = false;
    bool isReady=false;
};

class GnssDataProcess{
public:
    GnssDataProcess();
    ~GnssDataProcess();
    void dataProcess(const std::string &buffer);//判断报文类型，处理数据
    bool isReady();//发送数据是否准备好
    const std::string &getReadyData();//返回处理好的发送数据

private:
    double degreesConvert(const std::string &in_data1, const std::string &in_data2);//角度转换，暂时未使用
    void handleGGA(const std::string &data);//处理GGA报文
    void handleVTG(const std::string &data);//处理VTG报文
    GnssData m_gnssData;
    std::string m_readyData;//处理的发送数据
    
    std::queue<std::string> m_dataQue;//存放报文数据的数据队列
};