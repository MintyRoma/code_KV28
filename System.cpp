#include "System.h"

System::System(cl_base* parent, std::string name):cl_base(parent,name)
{

}

void System::build_tree_objects()
{
    cl_base* root = this;
    InputDevice Inp(this, "Input Device");
    Controller
}


