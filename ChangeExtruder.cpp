#include "ChangeExtruder.h"

ChangeExtruder::ChangeExtruder(cl_base* parent, std::string name) :cl_base(parent, name)
{

}

void ChangeExtruder::ChangeInformation(std::string command)
{
	if (command == "Status of the money refund device")
	{
		this->send_data(GET_SIGNAL_POINTER(ChangeExtruder::CollectChangeInfo), "");
	}
}

void ChangeExtruder::CollectChangeInfo(std::string& message)
{
	message = "Status of the money refund device:";
	cl_base* controller = this->find_by_filter((std::string)"//" + CONTROLLERDEVICE);
	SIGNAL_POINTER info_m = GET_SIGNAL_POINTER(Controller::GetCashInfo);
	std::string argument = "Change";
	(controller->*info_m)(argument);
	message += argument;
}