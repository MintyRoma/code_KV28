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
	while (argument != "")
	{
		int pos = argument.find_first_of(" ");
		std::string tickets = argument.substr(0, pos);
		int amount = std::stoi(tickets);
		Timetable[counter]->amount = amount;
		counter++;
	}
}

void TicketPrinter::FillPrice(std::string argument)
{
	int counter = 0;
	while (argument != "")
	{
		int pos = argument.find_first_of(" ");
		std::string tickets = argument.substr(0, pos);
		int price = std::stoi(tickets);
		Timetable[counter]->price = price;
		counter++;
	}
}