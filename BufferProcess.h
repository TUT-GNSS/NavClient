#pragma once
#include "serial/serial.h"

class BufferProcess{
public:
    BufferProcess(serial::Serial &serial);
    ~BufferProcess();
    virtual void handleBuffer();

protected:
    serial::Serial &m_serial;
};