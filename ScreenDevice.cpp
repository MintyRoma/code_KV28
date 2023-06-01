#include "ScreenDevice.h"

ScreenDevice::ScreenDevice(cl_base* parent, std::string name):cl_base(parent,name)
{
    
}

void ScreenDevice::PrintInformation(std::string info)
{
    if(info!="")std::cout << info;
}