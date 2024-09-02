#include "GnssBufferProcess.h"

GnssBufferProcess::GnssBufferProcess()
{
}
GnssBufferProcess::~GnssBufferProcess()
{

}

void GnssBufferProcess::handleBuffer()
{
    if (readSerial(1) == "$")
    {
        m_buffer.clear();
        do{
            m_buffer += readSerial(1);
        } while (m_buffer.back() != '\r' || readSerial(1) != "\n");
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