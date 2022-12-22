#include "parsing.h"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <ctime>
std::string Parsing::lowercase(std::string str) {
	std::size_t size = str.size();
	for (int i = 0; i < size; ++i) {
		if (str[i] >= 'A' && str[i] <= 'Z') {
			str[i] += 32;
		}
	}
	return str;
}

std::vector<std::string> Parsing::parser(const std::string& str, const std::string& spliter)
{
	if (str.empty()) {
		throw std::invalid_argument ("sorry text cant be empty");
	}
	std::vector<std::string> nstr;
	std::string tstr = "";
	for (int i = 0; i < str.size(); ++i)
	{
		if (spliter.find(str[i]) != std::string::npos)
		{   
            if (!tstr.empty())
            {
			    nstr.push_back(tstr);
			    tstr.clear();
			    continue;
            }
            continue;
		}
		tstr.push_back(str[i]);
	}
	if (!tstr.empty()) {
		nstr.push_back(tstr);
	}
	return nstr;
}

std::string Parsing::pars_data_from_file(const std::string& src) {
	if (src.size() < 1) {
		return "";
	}
	int i = src.size() - 1;
    while (src[i] != '#') {
        --i;
    }
    return src.substr(i + 2, src.size() - 1);
}

void Parsing::trim(std::string& src) {
	auto it = src.end() - 1;
	while (*it == ' ') {
		src.pop_back();
	}
	it = src.begin();
	while (*it == ' ') {
		src.erase(it);
	}
}

std::vector<std::string> Parsing::user_data_parser(const std::string& str, const std::string& spliter) {
	std::string src;
	int i = str.find('.');
	i += 5;
	int end = str.size();
	while (i < end) {
		src += str[i];
		++i;
	}
	auto parsed = parser(src, spliter);
	for (auto& iter : parsed) {
        trim(iter);
    }
	
	return parsed;	
}

//to_update parser
std::vector<std::string> Parsing::data_to_update(const std::string& src) {
	std::string low = lowercase(src);
	if (!low.find("where")) {
		throw std::invalid_argument ("missing keyword where\n");
	}
	int index_of_where = low.find("where");
	index_of_where += 5;
	std::string str = src.substr(index_of_where, src.size() - 1);
	auto parsed = parser(str, ",=");
	for (auto& iter : parsed) {
        trim(iter);
    }
	// std::copy(parsed.begin(), parsed.end(), std::ostream_iterator<std::string>(std::cout, "|"));
	return parsed;
}

//data parser to update the old one
std::vector<std::string> Parsing::data_for_update(const std::string& src) {
	std::string low = lowercase(src);
	if (!low.find("where")) {
		throw std::invalid_argument ("missing keyword where\n");
	}
	int index_of_where = low.find("where");
	std::string str = src.substr(0, index_of_where);
	std::vector<std::string> parsed = user_data_parser(str, ",=");
	for (auto& iter : parsed) {
        trim(iter);
    }
	// std::copy(parsed.begin(), parsed.end(), std::ostream_iterator<std::string>(std::cout, "|"));
	return parsed;
}

std::vector<std::string> Parsing::select_parser(const std::string& src) {
	std::string low = lowercase(src);
	if (low.find("where") == std::string::npos) {
		throw std::invalid_argument ("No match of keyword where, it's neccesary\n");
	}
	int where_index = low.find("where");
	where_index += 5;
	std::string data_part = src.substr(where_index, src.size() - 1);
	auto parsed = parser(data_part, ",=");
	for (auto& it : parsed) {
		trim(it);
	}
	if (low.find("age") != std::string::npos) {
		if (low.substr((low.find("age") + 3), low.size() - 1).find("age") != std::string::npos) {
			if (low.find('<') != std::string::npos && low.find('>') != std::string::npos) {
				return parsed;
			}
			else {
				throw std::invalid_argument("wrong syntax of command try again\n");
			}	
		}
	}
	// std::copy(parsed.begin(), parsed.end(), std::ostream_iterator<std::string>(std::cout, "|"));
	return parsed;
}

std::vector<std::string> Parsing::remove_sym_strings(std::vector<std::string>& data) {
	for (auto it = data.begin(); it < data.end();) {
		if ((*it).find('>') != std::string::npos || (*it).find('<') != std::string::npos) {
			data.erase(it);
			continue;
		} 
		++it;
	}
	return data;
}

bool Parsing::check_select(const std::string& src) {
	std::string low = lowercase(src);
	if (low.find("all") != std::string::npos) {
		if (low.find("where") == std::string::npos) {
			return true;
		}
	}
	return false;
}

std::vector<std::string> Parsing::task_parser(const std::string& str, const std::string& symbol) {
std::string tmp;
    std::vector <std::string> v;
    for (auto it = str.begin(); it <= str.end(); ++it) {
		if (*(it) == '<') {
			++it;
			while (*it != '>' && it != str.end()) {
				tmp += *it;
				++it;
			}
			v.push_back(tmp);
			tmp.clear();
			continue;
		}
        bool check = true;
        for (auto it1 = symbol.begin(); it1 < symbol.end(); ++it1) {
            if (*it == *it1 || it == str.end()) {
                check = false;
                if (!tmp.empty())
                {
                    v.push_back(tmp);
                    tmp.clear();
                }

                break;
            }
        }
        if(check)
        {
            tmp += *it;
        }
    }
	for (auto& it : v) {
		trim(it);
	}
    return v;
}

std::string Parsing::deadline_parser(const std::string& data) {
	auto parsed = parser(data, ".,/ ");
	if (parsed.size() != 3) {
		throw std::invalid_argument ("wrong date format");
	}
	std::time_t now = std::time(NULL);
	struct tm* date = std::localtime(&now);
	bool check;
	int day = std::stoi(parsed[0]);
	int month = std::stoi(parsed[1]);
	int year = std::stoi(parsed[2]);
	if (month == 4 || month == 6 || month == 9 || month == 11) {
		check = (day <= 30 && day >= 1) ? true : false;
	}
	else if (month == 2) {
		check = (day <= 28 && day >= 1) ? true : false;
	}
	else {
		check = (day <= 31 && day >= 1) ? true : false;
	}
	if (!check) {
		throw std::invalid_argument ("wrong day");
	}
	check = (year >= (date->tm_year + 1900)) ? true : false;
	if (check) {
		if (year == (date->tm_year + 1900)) {
			check = (month >= (date->tm_mon + 1)) ? true : false;
			if (check) {
				check = (day > date->tm_mday) ? true : false;
				if (check) {
					date->tm_year = year - 1900;
					date->tm_mday = day;
					date->tm_mon = month - 1;
				}
			}
		}
		if (year > (date->tm_year + 1900)) {
			date->tm_year = year - 1900;
			date->tm_mday = day;
			date->tm_mon = month - 1;
		}
	}
	if (!check) {
		throw std::invalid_argument ("wrong date");
	}
	std::string str = std::to_string(date->tm_mday) + '.' +std::to_string(date->tm_mon + 1) + '.' + std::to_string(date->tm_year + 1900);
	return str;
}

std::vector<std::size_t> Parsing::pars_from_file_for_ID(const std::string& str) {
	std::string parsed = pars_data_from_file(str);
	std::vector<std::size_t> data;
	std::string tmp;
	for (auto& it : parsed) {
		if (it =='|') {
			data.push_back(std::stoi(tmp));
			tmp.clear();
			continue;
		}
		tmp += it;
	}
	return data;
}

std::vector<std::string> Parsing::updateable_data_for_task(const std::string& src) {
	std::string low = lowercase(src);
	if (!low.find("where")) {
		throw std::invalid_argument ("missing keyword where\n");
	}
	int index_of_where = low.find("where");
	std::string str = src.substr(0, index_of_where);
	int file_indx = (str.find('.') + 5);
	str = str.substr(file_indx, str.size() - 1);
	std::vector<std::string> parsed = task_parser(str, ",=");
	for (auto& iter : parsed) {
        trim(iter);
    }
	// std::copy(parsed.begin(), parsed.end(), std::ostream_iterator<std::string>(std::cout, "|"));
	return parsed;
}

std::vector<std::string> Parsing::data_for_task_to_be_updated(const std::string& src) {
	std::string low = lowercase(src);
	if (!low.find("where")) {
		throw std::invalid_argument ("missing keyword where\n");
	}
	int index_of_where = low.find("where");
	index_of_where += 5;
	std::string str = src.substr(index_of_where, src.size() - 1);
	auto parsed = task_parser(str, ",=");
	for (auto& iter : parsed) {
        trim(iter);
    }
	// std::copy(parsed.begin(), parsed.end(), std::ostream_iterator<std::string>(std::cout, "|"));
	return parsed;
}

