#include "Controller.h"

Controller::Controller(cl_base* parent, std::string name) :cl_base(parent, name)
{
	CashDrawer = { { 5,0 }, { 10,0 }, { 50,0 }, { 100,0 }, { 200,0 }, { 500,0 }, { 1000,0 } };
}

void Controller::InitSessions(std::string command)
{
	if (command.substr(0, ((std::string)"InitSessions").size()) == "InitSessions")
	{
		std::string argument = command.substr(((std::string)"InitSessions").size());
		int amount = std::stoi(argument);
		for (int i = 0; i < amount; i++)
		{
			Timetable.push_back(new Session);
		}
	}
}

void Controller::FillSeats(std::string command)
{
	if (command.substr(0, ((std::string)"FillSeats").size()) == "FillSeats")
	{
		std::string argument = command.substr(((std::string)"FillSeats").size());

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
}

void Controller::FillPrice(std::string command)
{
	if (command.substr(0, ((std::string)"FillPrice").size()) == "FillPrice")
	{
		std::string argument = command.substr(((std::string)"FillPrice").size());

		int counter = 0;
		std::istringstream iss(argument);
		std::string part;
		while (getline(iss, part, ' '))
		{
			int number = std::stoi(part);
			Timetable[counter]->price = number;
			counter++;
		}
	}
}

void Controller::FillChange(std::string command)
{
	if (command.substr(0, ((std::string)"FillChange").size()) == "FillChange")
	{
		std::string argument = command.substr(((std::string)"FillChange").size());
		int nominals[6] = { 5,10,50,100,200,500 };
		int counter = 0;
		std::istringstream iss(argument);
		std::string part;
		while (getline(iss, part, ' '))
		{
			int number = std::stoi(part);
			CashDrawer[nominals[counter]] = number;
			counter++;
		}
	}
}

void Controller::GetCashInfo(std::string& argument)
{
	std::string info;
	for (std::map<int, int>::iterator iter = CashDrawer.begin(); iter != CashDrawer.end(); iter++)
	{
		if (iter->first == 1000 && argument == "Change") break;
		if (iter->second != 0)info += " " + std::to_string(iter->first) + ":" + std::to_string(iter->second) + ";";
	}
	argument = info;
}

void Controller::GetRevenue(std::string& argument)
{
	argument = std::to_string(revenue);
}

void Controller::GetUnsoldTickets(std::string& argument)
{
	int unsold = 0;
	for (Session* ses : Timetable)
	{
		unsold += ses->amount;
	}
	argument = std::to_string(unsold);
}

void Controller::GetChangeSum(std::string& argument)
{
	int sum = 0;
	for (std::map<int, int>::iterator iter = CashDrawer.begin(); iter != CashDrawer.end(); iter++)
	{
		if (iter->first != 1000) sum += iter->first * iter->second;
	}
	argument = std::to_string(sum);
}

void Controller::TicketReservationCommand(std::string command)
{
	if (command.substr(0, ((std::string)"Tickets").size()) == "Tickets" && this->mode == TicketSelection)
	{
		this->send_data(GET_SIGNAL_POINTER(Controller::ReserveTicket), command.substr(((std::string)"Tickets").size() + 1));
	}
}

void Controller::ReserveTicket(std::string& argument)
{
	int session = 0, amount = 0;
	bool first = true;
	std::istringstream iss(argument);
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
	reserved_amount = amount;
	reserved_session = Timetable[session - 1];

	Session* ses = Timetable[session - 1];
	if (ses->amount >= amount)
	{
		argument = "Number of tickets: " + std::to_string(reserved_amount) + "; Ticket price: " + std::to_string(reserved_amount * ses->price);
		mode = MoneyInsertion;
		if (!ChangePosibilityForDebit(reserved_session->price * amount))
		{
			argument = "Take the money back, no change";
			reserved_session = nullptr;
			reserved_amount = 0;
			mode = TicketSelection;
		}
	}
	else
	{
		argument = "No tickets";
		if (amount == 1) argument = "No ticket";
		reserved_session = nullptr;
		reserved_amount = 0;
		mode = TicketSelection;
	}

}

bool Controller::ChangePosibilityForDebit(int sum)
{
	int need = sum - (reserved_amount * reserved_session->price);
	if (need < 0) return true;
	for (std::map<int, int>::reverse_iterator iter = CashDrawer.rbegin(); iter != CashDrawer.rend(); iter++)
	{
		int nominal = iter->first;
		int amount = iter->second;
		if (need < iter->first || iter->first == 1000) continue;
		for (int i = 0; iter->second > i; i++)
		{
			if (need >= iter->first) need -= iter->first;
		}
	}
	if (need == 0) return true;
	return false;
}

void Controller::InsertionPrecheck(std::string& argument)
{
	if (mode == MoneyInsertion)
	{
		int sum = std::stoi(argument);
		if (!ChangePosibilityForDebit(sum * 2))
		{
			this->send_data(GET_SIGNAL_POINTER(Controller::Rejection), "");
		}
	}
	argument = "";
}

void Controller::RejectionCommand(std::string command)
{
	if (command == "Rejection" && mode == MoneyInsertion)
	{
		this->send_data(GET_SIGNAL_POINTER(Controller::Rejection), "");
	}
}

void Controller::Rejection(std::string& argument)
{
	cl_base* cashloader = this->find_by_filter(CASHLOADER);
	cashloader->send_data(GET_SIGNAL_POINTER(CashLoader::ReturnMoney), "");
	this->send_data(GET_SIGNAL_POINTER(Controller::ClearProcessing), "");
	argument = "";
}

void Controller::MoneyDisposing(std::string& argument)
{
	int nominal = std::stoi(argument);
	CashDrawer[nominal]--;

	argument = "";
}

void Controller::ClearProcessing(std::string& message)
{
	if (mode == TicketSelection)
	{
		message = "";
		return;
	}
	reserved_session = nullptr;
	reserved_amount = 0;
	mode = TicketSelection;
	message = "Ready to work";
}

void Controller::MoneyInsert(std::string& argument)
{
	if (mode == MoneyInsertion)
	{
		int nominal = std::stoi(argument);
		CashDrawer[nominal]++;
	}
	argument = "";
}

void Controller::MoneyInsertionNotify(std::string& argument)
{
	if (mode == MoneyInsertion)
	{
		int sum = std::stoi(argument);
		argument = "Ticket price " + std::to_string(reserved_amount * reserved_session->price) + " amount deposited " + std::to_string(sum);
		return;
	}
	argument = "";
}

void Controller::StartProcessing(std::string& argument)
{
	if (mode == MoneyInsertion)
	{
		int sum = std::stoi(argument);
		if (sum >= reserved_amount * reserved_session->price)
		{
			cl_base* ticketprinter = this->find_by_filter(TICKETPRINTER);
			ticketprinter->send_data(GET_SIGNAL_POINTER(TicketPrinter::GiveTicketsSignal), std::to_string(reserved_amount));
			std::string change = CreateChangeSet(sum - reserved_session->price * reserved_amount);
			if (change != "")change = std::to_string(sum - (reserved_amount * reserved_session->price)) + " =" + change;
			cl_base* changeextruder = this->find_by_filter(CHANGEEXTRUDER);
			changeextruder->send_data(GET_SIGNAL_POINTER(ChangeExtruder::ChangeCalculation), change);
			this->send_data(GET_SIGNAL_POINTER(Controller::CompleteProcessing), "");
			this->send_data(GET_SIGNAL_POINTER(Controller::ClearProcessing), "");
			cl_base* cashloader = this->find_by_filter(CASHLOADER);
			cashloader->send_data(GET_SIGNAL_POINTER(CashLoader::CompleteProcessing), "");
		}
	}
	argument = "";
}

std::string Controller::CreateChangeSet(int sum)
{
	std::map<int, int> cashset;
	for (std::map<int, int>::reverse_iterator iter = CashDrawer.rbegin(); iter != CashDrawer.rend(); iter++)
	{
		int nominal = iter->first;
		int amount = iter->second;
		if (sum == 0) break;
		if (sum < iter->first) continue;
		for (int i = 0; iter->second > i; i++)
		{
			if (sum - iter->first >= 0)
			{
				sum -= iter->first;
				cashset[iter->first]++;
			}
			else break;
		}
	}
	std::string new_mes;
	for (std::map<int, int>::reverse_iterator iter = cashset.rbegin(); iter != cashset.rend(); iter++)
	{
		if (iter->second != 0)new_mes += " " + std::to_string(iter->first) + " * " + std::to_string(iter->second) + " rub.";
		CashDrawer[iter->first] -= iter->second;
	}
	return new_mes;
}

void Controller::CompleteProcessing(std::string& argument)
{
	reserved_session->amount -= reserved_amount;
	revenue += reserved_session->price * reserved_amount;
}

void Controller::NumberOfTicketsCommand(std::string command)
{
	if (command == "Number of tickets")
	{
		this->send_data(GET_SIGNAL_POINTER(Controller::ShowTickets), "");
	}
}

void Controller::ShowTickets(std::string& message)
{
	std::string new_message = "Number of tickets: ";
	for (int i = 0; i < Timetable.size(); i++)
	{
		if (Timetable[i]->amount != 0)new_message += " (" + std::to_string(i + 1) + ":" + std::to_string(Timetable[i]->amount) + ")";
	}
	message = new_message;
}
