#include "ChangeDrawer.h"

ChangeDrawer::ChangeDrawer(cl_base* parent, std::string name):cl_base(parent,name)
{
	int types[6] = {5,10,50,100,200,500};
	for (int i = 0; i < 6; i++)
	{
		int amount;
		std::cin >> amount;
		CashStorage[types[i]]= amount;
	}
}
