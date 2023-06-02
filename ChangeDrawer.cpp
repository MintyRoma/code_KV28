#include "ChangeDrawer.h"

ChangeDrawer::ChangeDrawer(cl_base* parent, std::string name):cl_base(parent,name)
{
}

void ChangeDrawer::FillChange(std::string argument)
{
	int nominals[6] = { 5,10,50,100,200,500 };
	int counter = 0;
	std::istringstream iss(argument);
	std::string part;
	while (getline(iss, part, ' '))
	{
		int number = std::stoi(part);
		CashStorage[nominals[counter]] = number;
		counter++;
	}
}

void ChangeDrawer::GetStatusHandler(std::string command)
{
	if (command == "Status of the money refund device")
	{
		this->send_data(GET_SIGNAL_POINTER(ChangeDrawer::StatusSignal), "");
	}
}

void ChangeDrawer::StatusSignal(std::string & message)
{
	std::string new_message = "Status of the money refund device:";
	for (std::map<int, int>::iterator iter = CashStorage.begin(); iter != CashStorage.end(); iter++)
	{
		new_message += " "+std::to_string(iter->first) + ":" + std::to_string(iter->second) + ";";
	}
	message = new_message;
}

void ChangeDrawer::GetChangeTotal(std::string& message)
{
	int sum = 0;
	for (std::map<int, int>::iterator iter = CashStorage.begin(); iter != CashStorage.end(); iter++)
	{
		sum = iter->first * iter->second;
	}
	message += std::to_string(sum);
}
