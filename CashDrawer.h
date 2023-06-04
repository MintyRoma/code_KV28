#ifndef __CASHDRAWER__H
#define __CASHDRAWER__H

#include "cl_base.h"
#include <map>

class CashDrawer: public cl_base{
    
    private:
        std::map<int, int> CashStorage{ {5,0},{10,0},{50,0},{100,0},{200,0},{500,0},{1000,0} };
        std::map<int, int> DebitStorage{ {5,0},{10,0},{50,0},{100,0},{200,0},{500,0},{1000,0} };
    public:
        CashDrawer(cl_base* parent, std::string name);
        void GetStatusHandler(std::string command);
        void StatusSignal(std::string& message);
        void GetRevenueSignal(std::string& message);
        void MoneyInsertion(std::string& argument);
        void GetDebitSum(std::string& message);
};

#endif
