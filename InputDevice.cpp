#include "InputDevice.h"

InputDevice::InputDevice(cl_base* parent, std::string name):cl_base(parent,name)
{
    
}

void InputDevice::RequestInputHandler(std::string &result)
{
    std::string input;
    std::cin>>input;
    result=input;
}
