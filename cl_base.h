#ifndef __CL_BASE__H
#define __CL_BASE__H
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <exception>
#include <stack>

class cl_base;
typedef void (cl_base::* SIGNAL_POINTER)(std::string &); //Type: Signal method pointer
typedef void (cl_base::* HANDLER_POINTER)(std::string);
#define GET_SIGNAL_POINTER(signal_method) (SIGNAL_POINTER)(&signal_method)
#define GET_HANDLER_POINTER(handler_method) (HANDLER_POINTER)(&handler_method)

struct s_OBJECT_CHAT
{
	std::string name = "*";
	SIGNAL_POINTER p_signal;
	HANDLER_POINTER p_handler;
	cl_base* target;
};


class cl_base
{
private:
	//Methods

	//Data
	std::string s_object_name;
	cl_base* p_head_object;
	std::vector<cl_base*>  subordinate_objects;
	int status = 1;
	std::vector <s_OBJECT_CHAT*> links;


public:
	//Methods

	//V1
	cl_base(cl_base* p_head_object, std::string s_object_name = "Base_object");
	bool change_name(std::string s_object_name);
	std::string get_name();
	cl_base* get_parent();
	//V2
	void print_tree(int level = 0);
	cl_base* get_child_in_node(std::string child_name);
	cl_base* get_child_in_tree(std::string child_name);
	void set_status(int status);
	void print_tree_statuses(int level = 0);
	//V3
	bool change_parent(cl_base* new_parent);
	cl_base* find_by_filter(std::string query);
	bool remove_child(std::string child_name);
	//V4
	std::string get_path(std::string path = "");
	void create_link(SIGNAL_POINTER signal, HANDLER_POINTER handler, cl_base* target, std::string name="*");
	void remove_link(SIGNAL_POINTER signal, HANDLER_POINTER handler, cl_base* target, std::string name="*");
	void send_data(SIGNAL_POINTER signal, std::string message, std::string name = "*");
	void message_transmit_signal(std::string &message);
	void message_recieve_handler(std::string message);
	virtual int get_type();
	//Data
};


#endif
