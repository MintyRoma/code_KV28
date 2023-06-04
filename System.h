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
		void ReadyState(std::string command);
		void ConfirmReadiness(std::string& command);
		void ExitCommand(std::string command);
		void ConfirmExit(std::string& message);
		void StatusCommandHandler(std::string command);

		void CollectStatusInfo(std::string & message);
		
};

#endif
