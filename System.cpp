#include "System.h"
#include <conio.h>

System::System(cl_base* parent, std::string name):cl_base(parent,name)
{

}

void System::build_tree_objects()
{
    //Init no-input devices
    cl_base* inptdev = new InputDevice(this, INPUTDEVICE);
    cl_base* screendev = new ScreenDevice(this, SCREENDEVICE);
    cl_base* ctrldev = new Controller(this, CONTROLLERDEVICE);
    //Init Ticket printer
    cl_base* tckprint = new TicketPrinter(ctrldev, TICKETPRINTER);

    //Init Change Drawer
    cl_base* changedr = new ChangeExtruder(ctrldev,CHANGEEXTRUDER);
    
    cl_base* cashdr = new CashLoader(ctrldev, CASHLOADER);

    //Preparation
    inptdev->create_link(GET_SIGNAL_POINTER(InputDevice::CommandRead), GET_HANDLER_POINTER(Controller::InitSessions), ctrldev);
    inptdev->create_link(GET_SIGNAL_POINTER(InputDevice::CommandRead), GET_HANDLER_POINTER(Controller::FillSeats), ctrldev);
    inptdev->create_link(GET_SIGNAL_POINTER(InputDevice::CommandRead), GET_HANDLER_POINTER(Controller::FillPrice), ctrldev);
    inptdev->create_link(GET_SIGNAL_POINTER(InputDevice::CommandRead), GET_HANDLER_POINTER(Controller::FillChange), ctrldev);
    inptdev->create_link(GET_SIGNAL_POINTER(InputDevice::CommandRead), GET_HANDLER_POINTER(System::ReadyState), this);
    this->create_link(GET_SIGNAL_POINTER(System::ConfirmReadiness), GET_HANDLER_POINTER(ScreenDevice::PrintInformation), screendev);
    
    inptdev->create_link(GET_SIGNAL_POINTER(InputDevice::CommandRead), GET_HANDLER_POINTER(System::ExitCommand), this);
    this->create_link(GET_SIGNAL_POINTER(System::ConfirmExit), GET_HANDLER_POINTER(ScreenDevice::PrintInformation), screendev);
    
    inptdev->create_link(GET_SIGNAL_POINTER(InputDevice::CommandRead), GET_HANDLER_POINTER(ChangeExtruder::ChangeInformation), changedr);
    changedr->create_link(GET_SIGNAL_POINTER(ChangeExtruder::CollectChangeInfo), GET_HANDLER_POINTER(ScreenDevice::PrintInformation), screendev);

    inptdev->create_link(GET_SIGNAL_POINTER(InputDevice::CommandRead), GET_HANDLER_POINTER(CashLoader::CashInformation), cashdr);
    cashdr->create_link(GET_SIGNAL_POINTER(CashLoader::CollectCashInfo), GET_HANDLER_POINTER(ScreenDevice::PrintInformation), screendev);

    inptdev->create_link(GET_SIGNAL_POINTER(InputDevice::CommandRead), GET_HANDLER_POINTER(System::StatusCommandHandler), this);
    this->create_link(GET_SIGNAL_POINTER(System::CollectStatusInfo), GET_HANDLER_POINTER(ScreenDevice::PrintInformation), screendev);


    inptdev->create_link(GET_SIGNAL_POINTER(InputDevice::CommandRead), GET_HANDLER_POINTER(Controller::TicketReservationCommand), ctrldev);
    ctrldev->create_link(GET_SIGNAL_POINTER(Controller::ReserveTicket), GET_HANDLER_POINTER(ScreenDevice::PrintInformation), screendev);

    inptdev->create_link(GET_SIGNAL_POINTER(InputDevice::CommandRead), GET_HANDLER_POINTER(CashLoader::MoneyInsertionHandler), cashdr);
    cashdr->create_link(GET_SIGNAL_POINTER(CashLoader::MoneyInsertionConfirmation), GET_HANDLER_POINTER(ScreenDevice::PrintInformation), cashdr);
    ctrldev->create_link(GET_SIGNAL_POINTER(Controller::MoneyInsertionNotify), GET_HANDLER_POINTER(ScreenDevice::PrintInformation), screendev);
    ctrldev->create_link(GET_SIGNAL_POINTER(Controller::InsertMoneyPrecheck), GET_HANDLER_POINTER(ScreenDevice::PrintInformation), screendev);
    ctrldev->create_link(GET_SIGNAL_POINTER(Controller::TicketPaymentCheck), GET_HANDLER_POINTER(ScreenDevice::PrintInformation), screendev);
    cashdr->create_link(GET_SIGNAL_POINTER(CashLoader::MoneyTakeOut), GET_HANDLER_POINTER(ScreenDevice::PrintInformation), screendev);
    ctrldev->create_link(GET_SIGNAL_POINTER(Controller::CompleteProcessing), GET_HANDLER_POINTER(ScreenDevice::PrintInformation), screendev);
    changedr->create_link(GET_SIGNAL_POINTER(ChangeExtruder::ChangeCalculation), GET_HANDLER_POINTER(ScreenDevice::PrintInformation), screendev);
    cashdr->create_link(GET_SIGNAL_POINTER(CashLoader::CompleteProcessing), GET_HANDLER_POINTER(ScreenDevice::PrintInformation), screendev);
    //Init
    inptdev->send_data(GET_SIGNAL_POINTER(InputDevice::CommandRead), "InitSessions");
    inptdev->send_data(GET_SIGNAL_POINTER(InputDevice::CommandRead), "FillSeats");
    inptdev->send_data(GET_SIGNAL_POINTER(InputDevice::CommandRead), "FillPrice");
    inptdev->send_data(GET_SIGNAL_POINTER(InputDevice::CommandRead), "FillChange");
    inptdev->send_data(GET_SIGNAL_POINTER(InputDevice::CommandRead), "Ready");
}

int System::exec_app()
{
    cl_base* input_dev = this->find_by_filter((std::string)"//"+INPUTDEVICE);
    while (true)
    {
        input_dev->send_data(GET_SIGNAL_POINTER(InputDevice::CommandRead),"");
        tact++;
    }
    return 0;
}

void System::ReadyState(std::string command)
{
    if (command.substr(0, ((std::string)"Ready").size()) == "Ready")
    {
        std::string argument = command.substr(((std::string)"Ready").size());
        if (argument != "End of settings")
        {
            exit(0);
        }
        this->send_data(GET_SIGNAL_POINTER(System::ConfirmReadiness), "");
    }
}

void System::ConfirmReadiness(std::string& command)
{
    command = "Ready to work";
}

void System::ExitCommand(std::string command)
{
    if (command == "Turn off the system")
    {
        this->send_data(GET_SIGNAL_POINTER(System::ConfirmExit), "");
        exit(0);
    }
}

void System::ConfirmExit(std::string& message)
{
    message = "Turn off the system";
}

void System::StatusCommandHandler(std::string command)
{
    if (command == "System status")
    {
        this->send_data(GET_SIGNAL_POINTER(System::CollectStatusInfo), "");
    }
}

void System::CollectStatusInfo(std::string & message)
{
    cl_base* controller = this->find_by_filter((std::string)"//" + CONTROLLERDEVICE);
    SIGNAL_POINTER revenue_sig = GET_SIGNAL_POINTER(Controller::GetRevenue);
    SIGNAL_POINTER unsold_sig = GET_SIGNAL_POINTER(Controller::GetUnsoldTickets);
    SIGNAL_POINTER change_sig = GET_SIGNAL_POINTER(Controller::GetChangeSum);

    std::string tick, revenue, unsold, change;
    tick = std::to_string(this->tact);
    (controller->*revenue_sig)(revenue);
    (controller->*unsold_sig)(unsold);
    (controller->*change_sig)(change);

    message = "System status: tact " + tick + "; tickets " + unsold + "; ticket sales revenue " + revenue + "; change money " + change + ".";
}