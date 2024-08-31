#include "GnssBufferProcess.h"

GnssBufferProcess::GnssBufferProcess(serial::Serial &serial):BufferProcess(serial)
{
    // m_gnssDataProcess.setGetGGABufferCallback(std::bind(&GnssBufferProcess::getGGABuffer,this));
    // m_gnssDataProcess.setGetVTGBufferCallback(std::bind(&GnssBufferProcess::getVTGBuffer,this));
}
GnssBufferProcess::~GnssBufferProcess()
{

}
std::string GnssBufferProcess::getGGABuffer()
{
    std::string res = "GNGGA";
    // 读取GNGGA数据
    while (!(m_serial.read(1) == "\r" && m_serial.read(1) != "\n")) // 判断是否为截止符
    {
        res += m_serial.read(1);
    }
    return res;
}
std::string GnssBufferProcess::getVTGBuffer()
{
    std::string res = "GNVTG";
    //读取GNVTG数据
    while (!(m_serial.read(1) == "\r" && m_serial.read(1) != "\n"))//判断是否为截止符
    {
        res += m_serial.read(1);
    }
    return res;
}
void GnssBufferProcess::handleBuffer()
{
    if(m_serial.read(1)=="$"){
        m_buffer.clear();
        do{
            m_buffer+=m_serial.read(1);
        } while (m_buffer.back() == '\r' && m_serial.read(1) == "\n"); // 判断是否为截止符
        m_buffer.pop_back();
        // while (!(m_serial.read(1) == "\r" && m_serial.read(1) == "\n"))
        // {
        //     m_buffer += m_serial.read(1);
        // }
        m_gnssDataProcess.dataProcess(m_buffer);
        // std::string head =m_serial.read(5);
        // if(head=="GNGGA"){
        //     /*************/
        //     std::cout << "GGABuffer" << getGGABuffer() << std::endl;
        //     // getGGABuffer();
        // }
        // else if(head=="GNVTG"){
        //     /*************/
        //     std::cout << "VTGBuffer" << getVTGBuffer() << std::endl;
        //     // getVTGBuffer();
        // }
    }

}

bool GnssBufferProcess::isReady(){
    return m_gnssDataProcess.isReady();
}
const std::string & GnssBufferProcess::getReadyBuffer()
{
    return m_gnssDataProcess.getReadyData();
}