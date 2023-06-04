#ifndef __SYSTEM__H
#define __SYSTEM__H
#include "cl_base.h"
#include "KKMParts.h"

class System: public cl_base
{
	private:
		int tact = 1;
	public:
		//Constructor
		System(cl_base* parent, std::string name="System");
		//Methods
		void build_tree_objects();
		int exec_app();
		void ReadySetHandler(std::string command);
		void ReadySignal(std::string& message);
		void PowerOff(std::string info);
		void PowerOffSignal(std::string& message);
		void GetStatusHandler(std::string command);
		void GetStatusHandler(std::string& command);
		void GetStatusSignal(std::string& argument);
		void RequestSoldTicketsHandler(std::string& message);
		void RequestSalesRevenue(std::string& message);
		void RequestChangeSum(std::string & message);
};

#endif
