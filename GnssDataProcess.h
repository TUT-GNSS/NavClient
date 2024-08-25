#pragma once
#include <iostream>
#include <string>
#include <vector>


struct GnssData{
    std::string utcTime = "";     // #UTC时间
    std::string latitude = "";    // #latitude
    std::string lontitude = "";   // #longitude
    std::string numSatVisit = ""; // #可见卫星数
    std::string msl = "";  // #海拔高度Altitude，单位米（m）
    std::string cogt = ""; // #True north heading，单位degree  得到值+'T'+'°‘
    std::string cogm = ""; // #Magnetic north heading，单位degree（'°‘）
    std::string sog = ""; // #Ground speed，单位Kn
};

class GnssDataProcess{
public:
    GnssDataProcess();
    ~GnssDataProcess();
    void DataProcess();
private:
    void handleGnssData();
    GnssData gnssData;
};