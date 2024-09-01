#include "NavClient.h"

int main()
{
    DeviceInfo devInfo;
    devInfo.imuSerialPort = "/dev/ttyUSB0";
    devInfo.setImuInfoIsReady();
    devInfo.gnssSerialPort = "/dev/ttyUSB1";
    devInfo.setGnssInfoIsReady();

    devInfo.ip = "192.168.0.105";
    devInfo.socketPort = 5005;
    devInfo.setSocketInfoIsReady();
    NavClient navClient(devInfo);
    navClient.run();
    return 0;
}