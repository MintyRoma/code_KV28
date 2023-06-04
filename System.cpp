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

    //Preparation
    inptdev->create_link(GET_SIGNAL_POINTER(InputDevice::CommandRead), GET_HANDLER_POINTER(TicketPrinter::InitSessions), tckprint, "InitSessions");
    inptdev->create_link(GET_SIGNAL_POINTER(InputDevice::CommandRead), GET_HANDLER_POINTER(TicketPrinter::FillSeats), tckprint, "FillSeats");
    inptdev->create_link(GET_SIGNAL_POINTER(InputDevice::CommandRead), GET_HANDLER_POINTER(TicketPrinter::FillPrice), tckprint, "FillPrice");
    inptdev->create_link(GET_SIGNAL_POINTER(InputDevice::CommandRead), GET_HANDLER_POINTER(ChangeDrawer::FillChange), changedr, "FillChange");
    
    inptdev->create_link(GET_SIGNAL_POINTER(InputDevice::CommandRead), GET_HANDLER_POINTER(System::PowerOff), this); //Global Scope
    this->create_link(GET_SIGNAL_POINTER(System::PowerOffSignal), GET_HANDLER_POINTER(ScreenDevice::PrintInformation), screendev, "PowerOff");
    
    inptdev->create_link(GET_SIGNAL_POINTER(InputDevice::CommandRead), GET_HANDLER_POINTER(ChangeDrawer::GetStatusHandler), changedr);
    changedr->create_link(GET_SIGNAL_POINTER(ChangeDrawer::StatusSignal), GET_HANDLER_POINTER(ScreenDevice::PrintInformation), screendev);

    inptdev->create_link(GET_SIGNAL_POINTER(InputDevice::CommandRead), GET_HANDLER_POINTER(CashDrawer::GetStatusHandler), cashdr);
    cashdr->create_link(GET_SIGNAL_POINTER(CashDrawer::StatusSignal), GET_HANDLER_POINTER(ScreenDevice::PrintInformation), screendev);
    
    inptdev->create_link(GET_SIGNAL_POINTER(InputDevice::CommandRead), GET_HANDLER_POINTER(System::ReadySetHandler), this, "Ready");
    this->create_link(GET_SIGNAL_POINTER(System::ReadySignal), GET_HANDLER_POINTER(ScreenDevice::PrintInformation), screendev);

    inptdev->create_link(GET_SIGNAL_POINTER(InputDevice::CommandRead), GET_HANDLER_POINTER(TicketPrinter::NumberOfTicketsHandler), tckprint);
    tckprint->create_link(GET_SIGNAL_POINTER(TicketPrinter::TicketsAmountSignal), GET_HANDLER_POINTER(ScreenDevice::PrintInformation), screendev);

    inptdev->create_link(GET_SIGNAL_POINTER(InputDevice::CommandRead), GET_HANDLER_POINTER(System::GetStatusHandler), this);
    this->create_link(GET_SIGNAL_POINTER(System::GetStatusSignal), GET_HANDLER_POINTER(ScreenDevice::PrintInformation), screendev);
    
    //Init
    inptdev->send_data(GET_SIGNAL_POINTER(InputDevice::CommandRead), "", "InitSessions");
    inptdev->send_data(GET_SIGNAL_POINTER(InputDevice::CommandRead), "", "FillSeats");
    inptdev->send_data(GET_SIGNAL_POINTER(InputDevice::CommandRead), "", "FillPrice");
    inptdev->send_data(GET_SIGNAL_POINTER(InputDevice::CommandRead), "", "FillChange");
    inptdev->send_data(GET_SIGNAL_POINTER(InputDevice::CommandRead), "", "Ready");
}

int System::exec_app()
{
    cl_base* input_dev = this->find_by_filter("//Input Device");
    while (true)
    {
        input_dev->send_data(GET_SIGNAL_POINTER(InputDevice::CommandRead),"");
        tact++;
    }
    return 0;
}

void System::ReadySetHandler(std::string command)
{
    if (command == "End of settings")
    {
        this->send_data(GET_SIGNAL_POINTER(System::ReadySignal), "");
    }
    else exit(0);
}

void System::ReadySignal(std::string& message)
{
    message = "Ready to work";
}

void System::PowerOff(std::string argument)
{
    if (argument == "Turn off the system")
    {
        this->send_data(GET_SIGNAL_POINTER(System::PowerOffSignal), "", "PowerOff");
        exit(0);
    }
}

void System::PowerOffSignal(std::string& message)
{
    message = "Turn off the system";
}

void System::GetStatusHandler(std::string command)
{
    if (command == "System status")
    {
        this->send_data(GET_SIGNAL_POINTER(System::GetStatusSignal), "");
        
    }
}

void System::GetStatusSignal(std::string & argument)
{
    std::string current_tact = std::to_string(this->tact);
    std::string tickets_unsold, revenue, change;
    SIGNAL_POINTER req_reven = GET_SIGNAL_POINTER(CashDrawer::GetRevenueSignal);
    SIGNAL_POINTER req_change = GET_SIGNAL_POINTER(ChangeDrawer::GetChangeTotalSignal);
    SIGNAL_POINTER req_tick = GET_SIGNAL_POINTER(TicketPrinter::GetUnSoldTicketsSignal);
    cl_base* ticketprinter = this->find_by_filter("//Ticket Printer");
    cl_base* cashdraw = this->find_by_filter("//Cash Drawer");
    cl_base* changedraw = this->find_by_filter("//Change Drawer");
    (ticketprinter->*req_tick)(tickets_unsold);
    (cashdraw->*req_reven)(revenue);
    (changedraw->*req_change)(change);
    argument = "System status: tact " + current_tact + "; tickets " + tickets_unsold + "; ticket sales revenue " + revenue + "; change money " + change + ".";
}

