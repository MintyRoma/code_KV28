#include "cl_base.h"

cl_base::cl_base(cl_base* p_head_object = nullptr, std::string s_object_name)
{
	this->p_head_object = p_head_object;
	this->s_object_name = s_object_name;
	if (p_head_object) p_head_object->subordinate_objects.push_back(this);
}

bool cl_base::change_name(std::string s_object_name)
{
	if (this->s_object_name != s_object_name)
	{
		this->s_object_name = s_object_name;
		return true;
	}
	return false;
}

std::string cl_base::get_name()
{
	return s_object_name;
}

cl_base* cl_base::get_parent()
{
	return p_head_object;
}

//Modified V2
cl_base* cl_base::get_child_in_node(std::string child_name)
{
	if (this->get_name() == child_name) return this;
	for (cl_base* child : subordinate_objects)
	{
		if (child->get_name() == child_name) return child;
	}
	return nullptr;
}

//Modified V2
cl_base* cl_base::get_child_in_tree(std::string child_name)
{
	if (this->get_name() == child_name) return this;
	for (cl_base* child : subordinate_objects)
	{
		cl_base* ret = child->get_child_in_tree(child_name);
		if (ret != nullptr) return ret;
	}
	return nullptr;
}

//Modified V2
void cl_base::set_status(int status)
{
	cl_base* parent = this->get_parent();
	int stat = 1;
	if (parent != nullptr) stat = parent->status;
	if (status == 0 || stat == 0)
	{
		this->status = 0;
		for (cl_base* child : subordinate_objects) child->set_status(0);
	}
	else this->status = status;
}

//Modified V2
void cl_base::print_tree_statuses(int level)
{
	for (int i = 0; i < level; i++)std::cout << "    ";
	std::cout << get_name();
	if (this->status != 0) std::cout << " is ready";
	else std::cout << " is not ready";
	cl_base* parent = this->get_parent();
	for (cl_base* child : subordinate_objects)
	{
		std::cout << "\n";
		child->print_tree_statuses(level + 1);
	}
}

//Modified V2
void cl_base::print_tree(int level)
{
	for (int i = 0; i < level; i++)std::cout << "    ";
	std::cout << get_name();
	for (cl_base* child : subordinate_objects)
	{
		std::cout << "\n";
		child->print_tree(level + 1);
	}
}

//Modified V3
bool cl_base::change_parent(cl_base* new_parent)
{
	if (new_parent == nullptr)
	{
		throw std::runtime_error("Head object is not found");
		return false;
	}
	if (new_parent->get_child_in_node(this->get_name()) != nullptr)
	{
		throw std::runtime_error("Dubbing the names of subordinate objects");
		return false;
	}

	//Search parent in child
	cl_base* node = new_parent;
	while (node->get_parent() != nullptr)
	{
		if (node->get_parent() == this)
		{
			throw std::runtime_error("Redefining the head object failed");
			return false;
		}
		node = node->get_parent();
	}

	cl_base* old_parent = this->get_parent();

	std::vector<cl_base*> reconstruct;
	for (cl_base* child : old_parent->subordinate_objects)
	{
		if (child == this) continue;
		reconstruct.push_back(child);
	}
	old_parent->subordinate_objects = reconstruct;
	new_parent->subordinate_objects.push_back(this);
	p_head_object = new_parent;
	return true;
}

//Modified V3
bool cl_base::remove_child(std::string child_name)
{
	bool op_status = false;
	std::vector<cl_base*> new_vec;
	for (cl_base* child : this->subordinate_objects)
	{
		if (child->get_name() != child_name) new_vec.push_back(child);
		else
		{
			op_status = true;
			delete child;
		}
	}
	this->subordinate_objects = new_vec;

	return op_status;
}

//Modified V3
cl_base* cl_base::find_by_filter(std::string query)
{
	if (query[0] == '/')
	{
		cl_base* root = this;
		while (root->get_parent() != nullptr)root = root->get_parent();
		//Get root
		if (query.length() == 1)
		{
			return root;
		}

		//Find unique in tree
		else if (query[1] == '/')
		{
			std::vector<cl_base*> children;
			std::string name = query.substr(2);
			cl_base* operational = root;
			std::stack<cl_base*> stack;
			stack.push(root);
			while (!stack.empty())
			{
				cl_base* operational = stack.top();
				stack.pop();
				if (operational->get_name() == name)
				{
					children.push_back(operational);
				}
				for (cl_base* child : operational->subordinate_objects)
				{
					stack.push(child);
				}
			}
			if (children.size() == 1)return children.front();
			else return nullptr;
		}

		//Get by absolute path
		else
		{
			query = query.substr(1);
			std::vector<std::string> nodes;
			int pos = 0;
			while ((pos = query.find('/')) != std::string::npos)
			{
				std::string token = query.substr(0, pos);
				nodes.push_back(token);
				query.erase(0, pos + 1);
			}
			nodes.push_back(query);

			for (std::string node : nodes)
			{
				cl_base* found = root->get_child_in_node(node);
				if (found != nullptr) root = found;
				else return nullptr;
			}
			return root;
		}
	}
	else if (query[0] == '.')
	{
		if (query.length() == 1)
		{
			return this;
		}
		else
		{
			std::string child_name = query.substr(1);
			return this->get_child_in_tree(child_name);
		}
	}
	else
	{
		std::vector<std::string> nodes;
		int pos = 0;
		while ((pos = query.find('/')) != std::string::npos)
		{
			std::string token = query.substr(0, pos);
			nodes.push_back(token);
			query.erase(0, pos + 1);
		}
		nodes.push_back(query);
		cl_base* child = this;
		for (std::string node : nodes)
		{
			child = child->get_child_in_node(node);
		}
		return child;
	}
	return nullptr;
}

//Modified V4
std::string cl_base::get_path(std::string path)
{
	if (this->get_parent() == nullptr)
	{
		if (path == "") return "/";
		return path;
	}
	path = "/" + this->get_name() + path;
	return this->get_parent()->get_path(path);
}

//Modified V4
void cl_base::create_link(SIGNAL_POINTER signal, HANDLER_POINTER handler, cl_base* target, std::string name)
{
	s_OBJECT_CHAT* new_link;
	for (s_OBJECT_CHAT* link : this->links)
	{
		if (link->p_signal == signal && link->p_handler == handler && link->target == target && link->name==name)return;
	}
	new_link = new s_OBJECT_CHAT;
	new_link->p_signal = signal;
	new_link->p_handler = handler;
	new_link->target = target;
	links.push_back(new_link);
}

//Modified V4
void cl_base::remove_link(SIGNAL_POINTER signal, HANDLER_POINTER handler, cl_base* target, std::string name)
{
	for (int position=0; position<links.size() ; position++)
	{
		if (links[position]->p_signal == signal && links[position]->p_handler == handler && links[position]->target == target && links[position]->name==name)
		{
			delete links[position];
			links.erase(links.begin()+position);
			return;
		}
	}
}

//Modified V4
void cl_base::send_data(SIGNAL_POINTER signal,std::string name, std::string message)
{
	if(status==0)return;
	std::cout<<"\n";
	(this->*signal)(message);
	for (s_OBJECT_CHAT* link : this->links)
	{
		if (link->p_signal == signal && link->name==name)
		{
			cl_base* target = link->target;
			if(target->status==0) continue;
			std::cout << "\n";
			HANDLER_POINTER handler = link->p_handler;
			(target->*handler)(message);
		}
	}
}

//Modified V4
void cl_base::message_transmit_signal(std::string & message)
{
	int type = this->get_type();
	std::string new_message = message;
	new_message += " (class: " + std::to_string(type) + ")";
	std::cout << "Signal from " << this->get_path();
	
	*&message = new_message; //Присвоение update адреса message

	
}
//Modified V4
void cl_base::message_recieve_handler(std::string message)
{
	std::cout << "Signal to " << this->get_path() << " Text:  " << message;
	
}

//Modified V4
int cl_base::get_type()
{
	return 1;
}
