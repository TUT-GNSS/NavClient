#include "NavClient.h"
#include <signal.h>

NavClient *navClient = nullptr;
// 信号处理函数
void Stop(int sig)
{
    if (navClient)
    {
        /****其他数据释放操作****/
        delete navClient;
        printf("navClient已停止\n");
        exit(0);
    }
}

void setSignal()
{
    for (int i = 0; i <= 31; ++i)
    {
        signal(i, SIG_IGN);
    }
    signal(SIGTERM, Stop); // 捕获信号15, "killall"
    signal(SIGINT, Stop);  // 捕获信号2, "Ctrl+c"
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("IMU串口设备ttyUSB号 GNSS串口设备ttyUSB号  \n例如: ./testSerial 1 0\n\n");
        return -1;
    }
    setSignal();

    DeviceInfo devInfo;
    std::string serialAddr = "/dev/ttyUSB";

    devInfo.imuSerialPort = serialAddr + std::string(argv[1]);
    devInfo.setImuInfoIsReady();
    devInfo.gnssSerialPort = serialAddr + std::string(argv[2]);
    devInfo.setGnssInfoIsReady();

    navClient = new NavClient(devInfo);
    navClient->testSerial();
    return 0;
}