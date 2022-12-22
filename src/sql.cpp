#include "sql.h"

My_sql::My_sql() {
	// std::cout << "sql ctor" << std::endl;
  	init();
}

bool contains_file_name(const std::string& str) {
	return (str.find('.') != std::string::npos);
}

void My_sql::check_file_name(const std::string& str) {
	if (!static_cast<bool>(DB_map[str])) {
		std::cout << "failed" << std::endl;
		throw std::invalid_argument ("no such file or directory\n");
	}
}

void My_sql::execute(const std::string& src) {
	//parsing
	// std::cout << "exe" << std::endl;
	std::string split = " ,";
	auto pars = ob.parser(src, split);
	
	//checking and getting file name
    auto file_name = std::find_if(pars.begin(), pars.end(), contains_file_name);
	if (file_name == pars.end()) {
		throw std::invalid_argument("missing correct DB name");
	}
	// std::cout << *file_name << std::endl;
	*file_name = ob.lowercase(*file_name);
    this->check_file_name(*file_name);
	
	//get command name
    auto command_name = std::find_if(pars.begin(), (file_name), [&](std::string& src) { return (func.find(ob.lowercase(src)) != func.end());});
    command_name = ((command_name == file_name) ? (throw std::invalid_argument("No such command!")) : (command_name));
	*command_name = ob.lowercase(*command_name);
	// //get datas
    // std::vector<std::string> vec_datas(file_name + 1, pars.end());
    // //std::copy(std::begin(vec_datas), std::end(vec_datas), std::ostream_iterator<std::string>(std::cout, "||"));
    // //std::cout << std::endl;
    (this->*f_ptr[func[*command_name]])(objects[DB_map[*file_name]], src);
}


My_sql::~My_sql()
{
	// std::cout << "sql dtor" << std::endl;
	delete this->objects[DB_map["user.data"]];
	//delete this->objects[DB_map["task.data"]];
	delete this->objects[DB_map["project.data"]];
}

void My_sql::init()
{
	DB_map["user.data"] = DBfile::USER;
	DB_map["project.data"] = DBfile::PROJECT;
	DB_map["task.data"] = DBfile::TASK;

	this->objects[DBfile::USER] = new User();
	Connect* tmp = new Task(this->objects[DBfile::USER]);
	Connect* tmp_for_project = new Project(tmp, objects[DBfile::USER]);
	this->objects[DBfile::TASK] = tmp_for_project;
	this->objects[DBfile::PROJECT] = tmp_for_project;
	tmp = nullptr;
	tmp_for_project = nullptr;

	this->func["insert"] = Commands::INSERT;
	this->func["select"] = Commands::SELECT;
	this->func["delete"] = Commands::DELETE;
	this->func["update"] = Commands::UPDATE;

	this->f_ptr[Commands::INSERT] = &My_sql::insert;
	this->f_ptr[Commands::UPDATE] = &My_sql::update;
	this->f_ptr[Commands::DELETE] = &My_sql::del;
	this->f_ptr[Commands::SELECT] = &My_sql::select;
}

void My_sql::insert(Connect* connect, const std::string& data) {
	connect->insert(data);
	// std::cout << "con insert" << std::endl;
}

void My_sql::update(Connect* connect, const std::string& data) {
	connect->update(data);	
}

void My_sql::del(Connect* connect, const std::string& data) {
	connect->del(data);	
}

void My_sql::select(Connect* connect, const std::string& data) {
	std::vector<std::size_t> vec = connect->select(data);	
	auto parsed = ob.task_parser(data, " ,=");
	//checking and getting file name
    auto file_name = std::find_if(parsed.begin(), parsed.end(), [&](const std::string& input) {return input.find('.') != std::string::npos;});
    if (*file_name == "task.data") {
		connect->print_selected(vec, "task.data");
    }
	else {
		connect->print_selected(vec, "project.data");
	}
	
}