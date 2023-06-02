#ifndef __CHANGEDRAWER__H
#define __CHANGEDRAWER__H
#include <map>
#include "cl_base.h"
#include <sstream>

class ChangeDrawer: public cl_base{
    
    private:
        std::map<int, int> CashStorage{ {5,0},{10,0},{50,0},{100,0},{200,0},{500,0} };
    public:
        ChangeDrawer(cl_base* parent, std::string name);
        void FillChange(std::string argument);
        void GetStatusHandler(std::string command);
        void StatusSignal(std::string& message);
};

#endif
