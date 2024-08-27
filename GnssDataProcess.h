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
};

class GnssDataProcess{
public:
    GnssDataProcess();
    ~GnssDataProcess();
    void DataProcess(const std::string &in_data1);
    void setGGACallback(std::function<std::string()> fn);
    void setVTGCallback(std::function<std::string()> fn);

private:
    double degreesConvert(const std::string &in_data1, const std::string &in_data2);
    void handleGGA(const std::string &data);
    void handleVTG(const std::string &data);
    void handleGnssData();
    std::string m_slidingStr;
    GnssData m_gnssData;
    std::function<std::string()> m_getGGACallback;
    std::function<std::string()> m_getVTGCallback;
};