#include "GnssBufferProcess.h"

GnssBufferProcess::GnssBufferProcess(serial::Serial &serial):BufferProcess(serial)
{
    m_gnssDataProcess.setGetGGABufferCallback(std::bind(&GnssBufferProcess::getGGABuffer,this));
    m_gnssDataProcess.setGetVTGBufferCallback(std::bind(&GnssBufferProcess::getVTGBuffer,this));
}
GnssBufferProcess::~GnssBufferProcess()
{

}
std::string GnssBufferProcess::getGGABuffer()
{
    return m_serial.read(70);
}
std::string GnssBufferProcess::getVTGBuffer()
{
    return m_serial.read(40);
}
void GnssBufferProcess::handleBuffer()
{

}