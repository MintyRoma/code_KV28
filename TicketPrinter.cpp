#include "TicketPrinter.h"

TicketPrinter::TicketPrinter(cl_base* p_head_object, std::string name) :cl_base(p_head_object, name)
{
}

void TicketPrinter::InitSessions(std::string argument)
{
	int amount = std::stoi(argument);
	for (int i = 0; i < amount; i++)
	{
		Timetable.push_back(new Session);
	}
}

void TicketPrinter::FillSeats(std::string argument)
{
	int counter = 0;
	std::istringstream iss(argument);
	std::string part;
	while (getline(iss, part, ' '))
	{
		int number = std::stoi(part);
		Timetable[counter]->amount = number;
		counter++;
	}
}

void TicketPrinter::FillPrice(std::string argument)
{
	int counter = 0;
	std::istringstream iss(argument);
	std::string part;
	while (getline(iss, part, ' '))
	{
		int number = std::stoi(part);
		Timetable[counter]->price= number;
		counter++;
	}
}

void TicketPrinter::NumberOfTicketsHandler(std::string argument)
{
	if (argument == "Number of tickets")
	{
		this->send_data(GET_SIGNAL_POINTER(TicketPrinter::TicketsAmountSignal), "");
	}
}

void TicketPrinter::TicketsAmountSignal(std::string & message)
{
	std::string new_message="Number of tickets:";
	int counter = 1;
	for (Session* ses : Timetable)
	{
		new_message += " ("+std::to_string(counter) + ":" +std::to_string(ses->amount)+ ")";
		counter++;
	}
	message = new_message;
}

void TicketPrinter::GetUnSoldTicketsSignal(std::string& request)
{
	int sum = 0;
	for (Session* ses : Timetable)
	{
		sum += ses->amount;
	}
	request = std::to_string(sum);
}

void TicketPrinter::CheckTicketAvailabilitySignal(std::string& request)
{
	int session=0, amount=0;
	bool first = true;
	std::istringstream iss(request);
	std::string part;
	while (getline(iss, part, ' '))
	{
		int number = std::stoi(part);
		if (first)
		{
			session = number;
			first = false;
		}
		else
		{
			amount = number;
		}
	}
	Session* ses = Timetable[session - 1];
	cl_base* controller = this->find_by_filter((std::string)"//"+CONTROLLERDEVICE);
	if (ses->amount >= amount)
	{
		request = "Number of tickets : " + std::to_string(ses->amount) + "; Ticket price : " + std::to_string(ses->amount * ses->price);
		this->reserved_session = ses;
		this->reserved_amount = amount;
		controller->send_data(GET_SIGNAL_POINTER(Controller::ChangeModeSignal), "Enough");
	}
	else
	{
		request = "No tickets";
		if (amount == 1) request = "No ticket";
		controller->send_data(GET_SIGNAL_POINTER(Controller::ChangeModeSignal), "Less");
	}
}

void TicketPrinter::GetReservedSum(std::string& message)
{
	message = std::to_string(reserved_session->price * reserved_amount);
}