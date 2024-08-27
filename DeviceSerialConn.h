#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "serial/serial.h"
#include "ImuDataProcess.h"
enum class DeciveType
{
    IMU,
    GNSS
};

class DeviceSerialConn{
public:
    DeviceSerialConn(DeciveType type,std::string port,std::string baudrate,std::string timeout);
    ~DeviceSerialConn();
    void open();

private : 
    
};