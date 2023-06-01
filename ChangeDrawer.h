#ifndef __CHANGEDRAWER__H
#define __CHANGEDRAWER__H
#include <map>
#include "cl_base.h"

class ChangeDrawer: public cl_base{
    
    private:
        std::map<int, int> CashStorage;
    public:
        ChangeDrawer(cl_base* parent, std::string name);
};

#endif
