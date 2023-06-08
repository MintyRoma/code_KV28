#pragma once
#include "cl_base.h"
#include "KKMParts.h"

struct Session
{
	int price = 0;
	int amount = 0;
};

enum Mode
{
	TicketSelection,
	MoneyInsertion
};

class Controller : public cl_base
{
	private:
		std::vector<Session*> Timetable;
		std::map<int, int> CashDrawer;
		int revenue = 0;
		Mode mode = TicketSelection;
		Session* reserved_session = nullptr;
		int reserved_amount = 0;

	public:
		Controller(cl_base* parent, std::string name);
		void InitSessions(std::string command);
		void FillSeats(std::string command);
		void FillPrice(std::string command);
		void FillChange(std::string command);
		void GetCashInfo(std::string& argument);
		void GetRevenue(std::string& argument);
		void GetUnsoldTickets(std::string& argument);
		void GetChangeSum(std::string& argument);
		void TicketReservationCommand(std::string command);
		void ReserveTicket(std::string& argument);
		void MoneyInsertionNotify(std::string& argument);
		bool ChangePosibilityForDebit(int sum);
		void StartProcessing(std::string& argument);
		std::string CreateChangeSet(int sum);
		void CompleteProcessing(std::string& argument);
		void NumberOfTicketsCommand(std::string command);
		void ShowTickets(std::string& message);
		void InsertionPrecheck(std::string& argument);
		void RejectionCommand(std::string command);
		void Rejection(std::string& argument);
		void MoneyDisposing(std::string& argument);
		void ClearProcessing(std::string& argument);
		void MoneyInsert(std::string& argument);
};

