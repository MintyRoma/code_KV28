#include "CashDrawer.h"

CashDrawer::CashDrawer(cl_base* parent, std::string name):cl_base(parent,name)
{

}

void CashDrawer::GetStatusHandler(std::string command)
{
	if (command == "Status of the money receiving device")
	{
		this->send_data(GET_SIGNAL_POINTER(CashDrawer::StatusSignal));
	}
}

void CashDrawer::StatusSignal(std::string& message)
{
	std::string new_message = "Status of the money refund device:";
	for (std::map<int, int>::iterator iter = CashStorage.begin(); iter != CashStorage.end(); iter++)
	{
		new_message += " " + std::to_string(iter->first) + ":" + std::to_string(iter->second) + ";";
	}
	message = new_message;
}

void CashDrawer::GetRevenueHandler(std::string& message)
{
	int sum=0;
	for (std::map<int, int>::iterator iter = CashStorage.begin(); iter != CashStorage.end(); iter++)
	{
		sum += iter->first * iter->second;
	}
	message += std::to_string(sum);
}