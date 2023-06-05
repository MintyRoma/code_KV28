#include "TicketPrinter.h"

TicketPrinter::TicketPrinter(cl_base* parent, std::string name) :cl_base(parent, name)
{

}

void TicketPrinter::GiveTicketsSignal(std::string& argument)
{
	argument = "Take tickets " + argument;
}