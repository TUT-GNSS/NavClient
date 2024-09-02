#include "NavClient.h"

int main(int argc,char* argv[])
{
    if(argc!=3){
        perror("输入格式错误,输入'目的ip 目的端口号',例如：./navClient 192.168.0.106 5005");
        exit(-1);
    }
    DeviceInfo devInfo;
    devInfo.imuSerialPort = "/dev/ttyUSB1";
    devInfo.setImuInfoIsReady();
    devInfo.gnssSerialPort = "/dev/ttyUSB0";
    devInfo.setGnssInfoIsReady();

    devInfo.ip = std::string(argv[1]);
    devInfo.socketPort = atoi(argv[2]);
    devInfo.setSocketInfoIsReady();
    NavClient navClient(devInfo);
    navClient.run();
    return 0;
}