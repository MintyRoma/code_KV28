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

    inptdev->set_status(1);
    screendev->set_status(1);
    ctrldev->set_status(1);
    cashdr->set_status(1);
    changedr->set_status(1);
    tckprint->set_status(1);

    //Preparation
    
    this->create_link(GET_SIGNAL_POINTER(TicketPrinter::PrepareSessions),nullptr,tckprint);
    tckprint->create_link(GET_SIGNAL_POINTER(TicketPrinter::PrepareSessions),GET_HANDLER_POINTER(ScreenDevice::PrintInformation),screendev);
    //Init
}

int System::exec_app()
{
    this->cl_base::print_tree();
    return 0;
}


