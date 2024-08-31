#pragma once
#include <iostream>
#include <string>
#include <functional>
#include "GnssDataProcess.h"
#include "serial/serial.h"
#include "BufferProcess.h"

class GnssBufferProcess:BufferProcess
{
public:    
    GnssBufferProcess(serial::Serial &serial);
    ~GnssBufferProcess();
    std::string getGGABuffer();
    std::string getVTGBuffer();
    void handleBuffer() override;
    bool isReady() override;
    const std::string &getReadyBuffer() override;

private:
    std::string m_buffer;
    int m_checkSum = 0;
    GnssDataProcess m_gnssDataProcess;
};