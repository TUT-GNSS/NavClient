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
    if (argc != 5)
    {
        printf("输入格式错误!输入格式:   目的ip 目的端口号 IMU串口设备ttyUSB号 GNSS串口设备ttyUSB号  \n例如:    ./navClient 192.168.0.106 5005  1 0\n\n");
        return -1;
    }
    setSignal();

    DeviceInfo devInfo;
    std::string serialAddr = "/dev/ttyUSB";

    devInfo.ip = std::string(argv[1]);
    devInfo.socketPort = atoi(argv[2]);
    devInfo.setSocketInfoIsReady();
    devInfo.imuSerialPort = serialAddr + std::string(argv[3]);
    devInfo.setImuInfoIsReady();
    devInfo.gnssSerialPort = serialAddr + std::string(argv[4]);
    devInfo.setGnssInfoIsReady();

    navClient = new NavClient(devInfo);
    navClient->run();
    return 0;
}