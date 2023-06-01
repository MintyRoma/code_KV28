#include "System.h"
#include <conio.h>

System::System(cl_base* parent, std::string name):cl_base(parent,name)
{

}

void System::build_tree_objects()
{
    //Init no-input devices
    cl_base* inptdev = new InputDevice(this, "Input Device");
    cl_base* screendev = new ScreenDevice(this, "Screen Device");
    cl_base* ctrldev = new Controller(this, "Controller Device");
    //Init Ticket printer
    cl_base* tckprint = new TicketPrinter(ctrldev, "Ticket Printer");

    //Init Change Drawer
    cl_base* changedr = new ChangeDrawer(ctrldev,"Change Drawer");
    
    cl_base* cashdr = new CashDrawer(ctrldev, "Cash Drawer");

    inptdev->set_status(1);
    screendev->set_status(1);
    ctrldev->set_status(1);
    cashdr->set_status(1);
    changedr->set_status(1);
    tckprint->set_status(1);

    //Linking
    this->create_link(GET_SIGNAL_POINTER(System::ReadyStatus), GET_HANDLER_POINTER(ScreenDevice::PrintInformation), screendev);
    
    inptdev->create_link(GET_SIGNAL_POINTER(InputDevice::RequestInputSignal), GET_HANDLER_POINTER(System::PowerOffHandler), this);


    std::string ready_proof;
    std::cin.ignore();
    getline(std::cin, ready_proof);
    if (ready_proof != "End of settings")std::cout<<ready_proof;
        
    this->send_data(GET_SIGNAL_POINTER(System::ReadyStatus), "");
}

int System::exec_app()
{
    cl_base* inptdev = find_by_filter("//Input Device");
    cl_base* screendev = find_by_filter("//Screen Device");
    cl_base* ctrldev = find_by_filter("//Controller Device");
    cl_base* tckprint = find_by_filter("//Ticket Printer");
    cl_base* changedr = find_by_filter("//Change Drawer");
    cl_base* cashdr = find_by_filter("//Cash Drawer");
    while (true)
    {
        inptdev->send_data(GET_SIGNAL_POINTER(InputDevice::RequestInputSignal), "");
    }
    while (!kbhit());
    return 0;
}

void System::ReadyStatus(std::string& info)
{
    info = "Ready to work";
}

void System::PowerOffHandler(std::string info)
{
    if (info == "Turn off the system")
    {
        this->set_status(0);
        exit(0);
    }
}


