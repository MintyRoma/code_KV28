#include "Controller.h"

Controller::Controller(cl_base* parent, std::string name):cl_base(parent,name)
{

}


void Controller::TicketRequestHandler(std::string command)
{
	if (command.substr(0, ((std::string)"Ticket").size()) == "Ticket" && mode == TicketSelector)
	{
		cl_base* tick_printer = this->find_by_filter("//Ticket Printer");
		tick_printer->send_data(GET_SIGNAL_POINTER(TicketPrinter::CheckTicketAvailabilitySignal),command.substr(8));
	}
}

void Controller::MoneyInsertionHandler(std::string command)
{
	if (command.substr(0, ((std::string)"Money").size()) == "Money" && mode == MoneyInsertion)
	{

	}
}

void Controller::ChangeModeSignal(std::string& argument)
{
	if (argument == "Enough")
	{
		argument = "";
		this->mode = MoneyInsertion;
	}
	else if (argument == "Rejection")
	{
		argument = "Ready to work";
		this->mode = TicketSelector;
	}
	else if (argument == "Completed")
	{
		this->mode = TicketSelector;
		argument = "Ready to work";
	}
	else
	{
		argument = "";
		this->mode = TicketSelector;
	}
}