#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <functional>

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
    void dataProcess(const std::string &buffer);
    // void setGetGGABufferCallback(std::function<std::string()> fn);
    // void setGetVTGBufferCallback(std::function<std::string()> fn);
    bool isReady();
    const std::string &getReadyData();

private:
    double degreesConvert(const std::string &in_data1, const std::string &in_data2);
    void handleGGA(const std::string &data);
    void handleVTG(const std::string &data);
    // std::string m_slidingStr;
    GnssData m_gnssData;
    // std::function<std::string()> m_getGGABufferCallback;
    // std::function<std::string()> m_getVTGBufferCallback;
    std::string m_readyData;

};