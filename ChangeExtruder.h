#pragma once
#include "cl_base.h"
#include "KKMParts.h"
class ChangeExtruder : public cl_base
{
	private:
	
		
	public:
		ChangeExtruder(cl_base* parent, std::string name);
		void ChangeExtruder::ChangeInformation(std::string command);
		void CollectChangeInfo(std::string& message);
};

