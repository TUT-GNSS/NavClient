#include "GnssBufferProcess.h"

GnssBufferProcess::GnssBufferProcess(serial::Serial &serial):BufferProcess(serial)
{
    std::bind(&m_gnssDataProcess,GnssDataProcess::setGetGGABufferCallback,this,getGGABuffer);
    std::bind(&m_gnssDataProcess,GnssDataProcess::setGetVTGBufferCallback,this,getVTGBuffer);
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