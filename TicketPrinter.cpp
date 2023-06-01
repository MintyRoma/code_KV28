#include "TicketPrinter.h"

TicketPrinter::TicketPrinter(cl_base* p_head_object, std::string name) :cl_base(p_head_object, name)
{
	int amount = 0;
	Timetable.clear();
	std::cin >> amount;
	for (int i = 0; i < amount; i++)
	{
		Session* ns;
		ns = new Session;
		std::cin >> ns->amount;
		ns->price = 0;
		this->Timetable.push_back(ns);
	}
	for (int i = 0; i < amount; i++)
	{
		std::cin >> Timetable[i]->price;
	}
}

