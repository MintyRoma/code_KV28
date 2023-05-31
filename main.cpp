#include <stdlib.h>
#include <stdio.h>
#include "System.h"

int main()
{
	System ob_cl_application(nullptr);
	ob_cl_application.build_tree_objects();
	return ob_cl_application.exec_app();
}