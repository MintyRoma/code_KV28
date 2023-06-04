#include "CashLoader.h"

CashLoader::CashLoader(cl_base* parent, std::string name) :cl_base(parent, name)
{
}

void CashLoader::CashInformation(std::string command)
{
	if (command == "Status of the money receiving device")
	{
		this->send_data(GET_SIGNAL_POINTER(CashLoader::CollectCashInfo), "");
	}
}

void CashLoader::CollectCashInfo(std::string& message)
{
	message = "Status of the money receiving device:";
	cl_base* controller = this->find_by_filter((std::string)"//" + CONTROLLERDEVICE);
	SIGNAL_POINTER info_m = GET_SIGNAL_POINTER(Controller::GetCashInfo);
	std::string argument = "";
	(controller->*info_m)(argument);
	message += argument;
}

void CashLoader::MoneyInsertionHandler(std::string command)
{
	if (command.substr(0, ((std::string)"Money").size()) == "Money")
	{
		cl_base* controller = this->find_by_filter((std::string)"//" + CONTROLLERDEVICE);
		SIGNAL_POINTER moder = GET_SIGNAL_POINTER(Controller::GetMode);
		std::string result;
		(controller->*moder)(result);
		if (result == "TicketSelection") return;
		this->send_data(GET_SIGNAL_POINTER(CashLoader::MoneyInsertionConfirmation), command.substr(((std::string)"Money").size()));
	}
}

void CashLoader::MoneyInsertionConfirmation(std::string & argument)
{
	int nominal = std::stoi(argument);
	DebitStorage[nominal]++;
	cl_base* controller = this->find_by_filter((std::string)"//" + CONTROLLERDEVICE);
	SIGNAL_POINTER moder = GET_SIGNAL_POINTER(Controller::InsertMoneyPrecheck);
	(controller->*moder)(argument);
	controller->send_data(GET_SIGNAL_POINTER(Controller::MoneyInsertionNotify), std::to_string(DebitSum()));
	argument = "";
}

int CashLoader::DebitSum()
{
	int sum = 0;
	for (std::map<int, int>::iterator iter = DebitStorage.begin(); iter != DebitStorage.end(); iter++)
	{
		sum += iter->first * iter->second;
	}
	return sum;
}