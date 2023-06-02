#ifndef __INPUTDEVICE__H
#define __INPUTDEVICE__H
#include "cl_base.h"
class InputDevice: public cl_base{

    private:

    public:
        InputDevice(cl_base* parent, std::string name);

        void CommandRead(std::string& result);

};

#endif
