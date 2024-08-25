#pragma once
#include <iostream>
#include <string>
#include <vector>
// #include <serial/serial.h>
#include <vector>
#include <algorithm> 
#include "serial/serial.h"
#include "ImuDataProcess.h"

void imu()
{
}

bool isStart = true;
int CheckSum = 0;
int data_length = 0, buf_length = 11;
std::vector<short> RxBuff(buf_length);

void GetDataDeal(std::vector<short>& list_buf, ImuDataProcess &imuDataProcess)
{
    if (list_buf[buf_length - 1] != CheckSum)
    {
        return;
    }
        imuDataProcess.DataProcess(list_buf);
}

void DueData(short inputData, ImuDataProcess &imuDataProcess)
{
    if (inputData == 0x55 && isStart) 
    {
        isStart = false;
        data_length = 11;
        CheckSum = 0;
        std::fill(RxBuff.begin(), RxBuff.end(), 0);
    }
    if (isStart == false)
    {
        CheckSum += inputData;
        RxBuff[buf_length - data_length] = inputData;
        data_length = data_length - 1;
        if (data_length == 0)
        {
            CheckSum = (CheckSum - inputData) & 0xff;
            isStart = true;
        }
        GetDataDeal(RxBuff, imuDataProcess);
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

    ImuDataProcess imuDataProcess;
    while (true)
    {
        // std::cout << "number" << ser.available() << std::endl; // 读取到缓存区数据的字节数
        DueData((short)ser_imu.read(1)[0],imuDataProcess);
    }
    return 0;
}