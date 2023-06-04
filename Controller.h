#ifndef __CONTROLLER__H
#define __CONTROLLER__H
#include "cl_base.h"
#include <sstream>
#include "KKMParts.h"

enum Mode
{
    TicketSelector,
    MoneyInsertion
};
class Controller: public cl_base{
    
    private:
        Mode mode = TicketSelector;
    public:
        Controller(cl_base* parent, std::string name);
        void TicketReservationSignal(std::string& argument);
        void TicketRequestHandler(std::string command);
        void MoneyInsertionHandler(std::string command);
        void ChangeModeSignal(std::string& argument);
        void MoneyInsertionResult(std::string& message);
        void TicketSelectedSignal(std::string& argument);
};

#endif
