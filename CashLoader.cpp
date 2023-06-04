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