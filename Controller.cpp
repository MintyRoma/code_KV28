#include "Controller.h"

Controller::Controller(cl_base* parent, std::string name):cl_base(parent,name)
{

}


void Controller::TicketRequestHandler(std::string command)
{
	if (command.substr(0, ((std::string)"Ticket").size()) == "Ticket" && mode == TicketSelector)
	{
		cl_base* tick_printer = this->find_by_filter((std::string)"//"+TICKETPRINTER);
		tick_printer->send_data(GET_SIGNAL_POINTER(TicketPrinter::CheckTicketAvailabilitySignal),command.substr(8));
	}
}

void Controller::MoneyInsertionHandler(std::string command)
{
	if (command.substr(0, ((std::string)"Money").size()) == "Money" && mode == MoneyInsertion)
	{
		cl_base* cash_drawer = this->find_by_filter((std::string)"//"+CASHDRAWER);
		SIGNAL_POINTER insertion = GET_SIGNAL_POINTER(CashDrawer::MoneyInsertion);
		(cash_drawer->*insertion)(command.substr(((std::string)"Money").size() + 1));
		this->send_data(GET_SIGNAL_POINTER(Controller::MoneyInsertionResult),"");
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

void Controller::MoneyInsertionResult(std::string & message)
{
	cl_base* cashdr = this->find_by_filter((std::string)"//"+CASHDRAWER);
	SIGNAL_POINTER debitsum = GET_SIGNAL_POINTER(CashDrawer::GetDebitSum);
	std::string sum_str, price_str;
	(cashdr->*debitsum)(sum_str);
	cl_base* ticketprint = this->find_by_filter((std::string)"//" + TICKETPRINTER);
	SIGNAL_POINTER ticketsum = GET_SIGNAL_POINTER(TicketPrinter::GetReservedSum);
	(ticketprint->*ticketsum)(price_str);

	message = "Ticket price " + price_str + " amount deposited " + sum_str;
}