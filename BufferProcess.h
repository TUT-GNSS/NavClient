#pragma once
#include "serial/serial.h"

class BufferProcess{
public:
    BufferProcess(serial::Serial &serial);
    ~BufferProcess();
    virtual void handleBuffer()=0;
    virtual bool isReady()=0;
    virtual const std::string &getReadyBuffer()=0;

protected:
    serial::Serial &m_serial;
};