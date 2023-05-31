#include "TicketPrinter.h"

TicketPrinter::TicketPrinter(cl_base* p_head_object, std::string name) :cl_base(p_head_object, name)
{
	
}

void TicketPrinter::PrepareSessions(std::string &message)
{
	int amount = 0;
	std::cin>>amount;
	for(int i=0;i<amount;i++)
	{
		Session ns;
		std::cin>>ns.amount;
		Timetable.push_back(ns);
	}
	for (int i=0;i<amount;i++)
	{
		std::cin>>Timetable[i].price;
	}
	send_data(nullptr,"Ready to work");
}
