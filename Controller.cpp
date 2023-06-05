#include "Controller.h"

Controller::Controller(cl_base* parent, std::string name) :cl_base(parent, name)
{

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
		info += " " + std::to_string(iter->first) + ":" + std::to_string(iter->second) + ";";
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
	argument = sum;
}

void Controller::TicketReservationCommand(std::string command)
{
	if (command.substr(0, ((std::string)"Tickets").size()) == "Tickets" && this->mode==TicketSelection)
	{
		this->send_data(GET_SIGNAL_POINTER(Controller::ReserveTicket), command.substr(((std::string)"Tickets").size()+1));
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
		argument = "Number of tickets : " + std::to_string(ses->amount) + "; Ticket price : " + std::to_string(ses->amount * ses->price);
		this->reserved_session = ses;
		this->reserved_amount = amount;
		mode = MoneyInsertion;
		if (!CheckChangePosibility())
		{
			argument = "Take the money back, no change";
			mode = TicketSelection;
		}
	}
	else
	{
		argument = "No tickets";
		if (amount == 1) argument = "No ticket";
		mode = TicketSelection;
	}
	
}

bool Controller::CheckChangePosibility()
{
	int sum = 0;
	for (std::map<int, int>::iterator iter = CashDrawer.begin(); iter != CashDrawer.end(); iter++)
	{
		if (sum > reserved_amount * reserved_session->price)
		{
			return true;
		}
		sum += iter->first * iter->second;
	}
	return false;
}

void Controller::GetMode(std::string& argument)
{
	if (mode == TicketSelection)
	{
		argument = "TicketSelection";
	}
	else
	{
		argument = "MoneyInsertion";
	}
}


void Controller::InsertMoneyPrecheck(std::string& argument)
{
	int nominal = std::stoi(argument);
	if (!ChangePosibilityForDebit())
	{
		cl_base* change_drawer = this->find_by_filter((std::string)"//" + CASHLOADER);
		change_drawer->send_data(GET_SIGNAL_POINTER(CashLoader::MoneyTakeOut), "");
		this->send_data(GET_SIGNAL_POINTER(Controller::CompleteProcessing), "0");
		argument = "";
		return;
	}
	CashDrawer[nominal]++;
}

void Controller::MoneyInsertionNotify(std::string& argument)
{
	std::string debit = argument;
	if (mode==MoneyInsertion)argument = "Ticket price " + std::to_string(reserved_amount * reserved_session->price) + " amount deposited " + debit;
	mode = TicketSelection;
}

void Controller::TicketPaymentCheck(std::string& argument)
{
	int sum = std::stoi(argument);
	if (sum < reserved_amount * reserved_session->price)
	{
		argument = "";
		return;
	}
	else
	{
		argument = "Take tickets " + std::to_string(reserved_amount);
		if (sum > reserved_amount * reserved_session->price)
		{
			cl_base* changedr = this->find_by_filter((std::string)"//" + CHANGEEXTRUDER);
			changedr->send_data(GET_SIGNAL_POINTER(ChangeExtruder::ChangeCalculation), std::to_string(sum - reserved_amount * reserved_session->price));
		}
		cl_base* loader = this->find_by_filter((std::string)"//" + CASHLOADER);
		loader->send_data(GET_SIGNAL_POINTER(CashLoader::CompleteProcessing), "");
		this->send_data(GET_SIGNAL_POINTER(Controller::CompleteProcessing), std::to_string(sum));
	}
}

void Controller::MoneyGiveBack(std::string& argument)
{
	int nominal = std::stoi(argument);
	CashDrawer[nominal]--;
}

bool Controller::ChangePosibilityForDebit()
{
	cl_base* loader = this->find_by_filter((std::string)"//" + CASHLOADER);
	SIGNAL_POINTER sig = GET_SIGNAL_POINTER(CashLoader::DebitSumSignal);
	std::string param;
	(loader->*sig)(param);
	int amount = std::stoi(param);
	int need = amount - (reserved_amount*reserved_session->price);
	for (std::map<int, int>::iterator iter = CashDrawer.end(); iter != CashDrawer.begin(); iter--)
	{
		if (need == 0) return true;
		for (int i = 0; iter->second > i; i++)
		{
			if (need >= iter->first) need -= iter->first;
		}
	}
	return false;
}

void Controller::CreateMoneySet(std::string & argument)
{
	int need = std::stoi(argument);
	std::map<int, int> cashset;
	for (std::map<int, int>::iterator iter = CashDrawer.end(); iter != CashDrawer.begin(); iter--)
	{
		if (need == 0) break;
		for (int i = 0; iter->second > i; i++)
		{
			if (need - iter->first > 0)
			{
				need -= iter->first;
				cashset[iter->first]++;
			}
		}
	}
	std::string new_mes;
	for (std::map<int, int>::iterator iter = cashset.end(); iter != cashset.begin(); iter--)
	{
		new_mes += " " + std::to_string(iter->first) + " * " + std::to_string(iter->second) + " rub.";
		CashDrawer[iter->first] -= iter->second;
	}
	argument = new_mes;
}

void Controller::CompleteProcessing(std::string& message)
{
	reserved_session = nullptr;
	reserved_amount = 0;
	mode = TicketSelection;
	revenue += std::stoi(message);
	message = "Ready to work";
}
