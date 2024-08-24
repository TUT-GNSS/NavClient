#include <iostream>
#include <string>
#include <vector>
// #include <serial/serial.h>
#include "serial/serial.h"
#include <vector>

void imu()
{
}

// struct data_package
// {
//     char start = 's';
//     char unused1[2];
//     float speed = 20;
//     float euler[3] = {}; //(0,1,2) = (yaw,roll,pitch)
//     char shoot_bool = 0;
//     char RuneFlag = 0; //
//     char unused2[10] = {};
//     char end = 'e';
// } __attribute__((packed));
// static_assert(sizeof(data_package) == 32);

// data_package data;

int start = 0;
int CheckSum = 0;
int data_length = 0, buf_length = 11;
std::vector<short> RxBuff(buf_length);

void GetDataDeal(std::vector<short> list_buf)
{
    if (list_buf[buf_length - 1] != CheckSum)
    {
        return;
    }
    if (list_buf[1] == 0x51)
    {
        std::cout << "加速度输出\n";
    }
}

void DueData(short inputData)
{
    if (inputData == 0x55 && start == 0)
    {
        start = 1;
        data_length = 11;
        CheckSum = 0;
        for (auto &a : RxBuff)
        {
            a = 0;
        }
    }
    if (start == 1)
    {
        CheckSum += inputData;
        RxBuff[buf_length - data_length] = inputData;
        data_length = data_length - 1;
        if (data_length == 0)
        {
            CheckSum = (CheckSum - inputData) & 0xff;
            start = 0;
        }
        GetDataDeal(RxBuff);
    }
}

int main()
{
    serial::Serial ser_imu;          // 实例化一个串口的对象
    ser_imu.setPort("/dev/ttyUSB0"); // 设置串口设备
    ser_imu.setBaudrate(115200);     // 设置波特率
    serial::Timeout to = serial::Timeout::simpleTimeout(1000);
    ser_imu.setTimeout(to);

    ser_imu.open(); // 打开串口
    while (true)
    {
        // std::cout << "number" << ser.available() << std::endl; // 读取到缓存区数据的字节数
        DueData((short)ser_imu.read(1)[0]);
    }
}