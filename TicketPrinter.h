#pragma once
#include "cl_base.h"
#include "KKMParts.h"

class TicketPrinter : public cl_base
{
	private:
		
	public:
		TicketPrinter(cl_base* parent, std::string name);
		void GiveTicketsSignal(std::string& argument);
};

