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
		this->send_data(GET_SIGNAL_POINTER(CashLoader::MoneyInsertionConfirmation), command.substr(((std::string)"Money").size()));
	}
}

void CashLoader::MoneyInsertionConfirmation(std::string & argument)
{
	int nominal = std::stoi(argument);
	DebitStorage[nominal]++;
	cl_base* controller = this->find_by_filter((std::string)"//" + CONTROLLERDEVICE);
	int sum = DebitSum();
	controller->send_data(GET_SIGNAL_POINTER(Controller::InsertionPrecheck),std::to_string(sum));
	controller->send_data(GET_SIGNAL_POINTER(Controller::MoneyInsert), std::to_string(nominal));
	controller->send_data(GET_SIGNAL_POINTER(Controller::MoneyInsertionNotify), std::to_string(sum));
	controller->send_data(GET_SIGNAL_POINTER(Controller::StartProcessing), std::to_string(sum));
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

void CashLoader::CompleteProcessing(std::string& message)
{
	for (std::map<int, int>::iterator iter = DebitStorage.begin(); iter != DebitStorage.end(); iter++) iter->second = 0;
	message = "";
}

void CashLoader::ReturnMoney(std::string& message)
{
	std::string new_message = "Take the money: " + std::to_string(DebitSum()) + " =";
	cl_base* controller = this->find_by_filter((std::string)"//" + CONTROLLERDEVICE);
	for (std::map<int, int>::reverse_iterator iter = DebitStorage.rbegin(); iter != DebitStorage.rend(); iter++)
	{
		if (iter->second!=0)new_message += " " + std::to_string(iter->first) + " * " + std::to_string(iter->second) + "rub.";
		for (int i = 0; i < iter->second; i++)
		{
			controller->send_data(GET_SIGNAL_POINTER(Controller::MoneyDisposing), std::to_string(iter->first));
		}
	}
	this->send_data(GET_SIGNAL_POINTER(CashLoader::CompleteProcessing), "");
	message = new_message;
}