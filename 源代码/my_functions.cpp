#include "my_functions.h"
my_functions::my_functions(void)
{
}

my_functions::~my_functions(void)
{
}
string my_functions::my_split(string in_string,char split_ch,int index)
{
	vector<string> split_str_vector;
	split_str_vector.clear();
	string::size_type loc_start = in_string.find_first_not_of(split_ch);
	string::size_type loc_stop = loc_start;
	while(loc_stop != string::npos) {
		loc_stop = in_string.find_first_of(split_ch, loc_start);
		if (loc_stop != string::npos)
		{
			split_str_vector.push_back(in_string.substr(loc_start, loc_stop - loc_start));
		}
		else
		{
			split_str_vector.push_back(in_string.substr(loc_start));
		}
		loc_start = in_string.find_first_not_of(split_ch, loc_stop);
		loc_stop = loc_start;
	}
	if(split_str_vector.size()>0)
	{
		if(index < 0 || index > split_str_vector.size()-1)
		{
			return split_str_vector[split_str_vector.size()-1];
		}
		else
		{
			return split_str_vector[index];
		}
	}
	else
	{
		return "";
	}
}