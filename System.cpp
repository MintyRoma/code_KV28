#include "System.h"

System::System(cl_base* parent, std::string name):cl_base(parent,name)
{

}

void System::build_tree_objects()
{
    cl_base* inptdev = new InputDevice(this, "Input Device");
    cl_base* screendev = new ScreenDevice(this, "Screen Device");
    cl_base* ctrldev = new Controller(this, "Controller Device");
    cl_base* cashdr = new CashDrawer(ctrldev, "Cash Drawer");
    cl_base* changedr = new ChangeDrawer(ctrldev,"Change Drawer");
    cl_base* tckprint = new TicketPrinter(ctrldev, "Ticket Printer");

    
    
}

int System::exec_app()
{
    this->cl_base::print_tree();
}


