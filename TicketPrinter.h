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
		std::vector<Session> Timetable;
	public:
		TicketPrinter(cl_base* p_head_object, std::string name);
		//Methods
		void PrepareSessions();	
		//Handlers
		
		//Signals
};

#endif
