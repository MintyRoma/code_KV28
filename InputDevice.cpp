#include "InputDevice.h"

InputDevice::InputDevice(cl_base* parent, std::string name):cl_base(parent,name)
{
    
}

void InputDevice::CommandRead(std::string &result)
{
    std::string input;
    getline(std::cin,input);
    result=input;
}
