#ifndef __TICKETPRINTER__H
#define __TICKETPRINTER__H
#include "cl_base.h"
#include <sstream>

	
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
		int sold = 0;
		//Methods

	public:
		TicketPrinter(cl_base* p_head_object, std::string name);
		void InitSessions(std::string argument);
		void FillSeats(std::string argument);
		void FillPrice(std::string argument);
		void NumberOfTicketsHandler(std::string argument);
		void TicketsAmountSignal(std::string& message);
		void GetUnSoldTicketsSignal(std::string& message);
		//Methods
		// 
		//Handlers
		//Signals
};

#endif
