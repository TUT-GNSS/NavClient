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

private:
    GnssDataProcess m_gnssDataProcess;
    // serial::Serial &m_serial;
};