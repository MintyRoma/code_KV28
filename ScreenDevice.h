#ifndef __SCREENDEVICE__H
#define __SCREENDEVICE__H

#include "cl_base.h"

class ScreenDevice: public cl_base{
    
    private:

    public:
		ScreenDevice(cl_base* parent, std::string name);

		void PrintInformation(std::string info);
};

#endif
