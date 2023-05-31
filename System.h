#ifndef __SYSTEM__H
#define __SYSTEM__H
#include "cl_base.h"
#include "KKMParts.h"

class System: public cl_base
{
	private:
	
	public:
		//Constructor
		System(cl_base* parent, std::string name="System");
		//Methods
		void build_tree_objects();
		int exec_app();
		void SignalExecutor();
};

#endif
