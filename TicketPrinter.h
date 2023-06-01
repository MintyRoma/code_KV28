#ifndef __TICKETPRINTER__H
#define __TICKETPRINTER__H
#include "cl_base.h"

	
struct Session
{
	int price;
	int amount;
};

class TicketPrinter: public cl_base
{
	private:
		//Vars
		std::vector <Session*> Timetable;
		//Methods

	public:
		TicketPrinter(cl_base* p_head_object, std::string name);
		//Methods
		// 
		//Handlers
		
		//Signals
};

#endif
