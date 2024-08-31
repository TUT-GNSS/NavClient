#include "GnssBufferProcess.h"

GnssBufferProcess::GnssBufferProcess(serial::Serial &serial):BufferProcess(serial)
{
    // m_gnssDataProcess.setGetGGABufferCallback(std::bind(&GnssBufferProcess::getGGABuffer,this));
    // m_gnssDataProcess.setGetVTGBufferCallback(std::bind(&GnssBufferProcess::getVTGBuffer,this));
}
GnssBufferProcess::~GnssBufferProcess()
{

}

void GnssBufferProcess::handleBuffer()
{
    if(m_serial.read(1)=="$"){
        m_buffer.clear();
        do{
            m_buffer += m_serial.read(1);
        } while (m_buffer.back()!='\r'||m_serial.read(1)!="\n");
        m_gnssDataProcess.dataProcess(m_buffer);
    }

}

bool GnssBufferProcess::isReady(){
    return m_gnssDataProcess.isReady();
}
const std::string & GnssBufferProcess::getReadyBuffer()
{
    return m_gnssDataProcess.getReadyData();
}