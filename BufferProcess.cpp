#include "BufferProcess.h"


BufferProcess::BufferProcess(){

}
BufferProcess::~BufferProcess(){

}

std::string BufferProcess::readSerial(size_t sz)
{
    return m_serialReadCallback(sz);
}

void BufferProcess::setReadCallback(std::function<std::string(size_t)> fn)
{
    m_serialReadCallback = fn;
}