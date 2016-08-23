#ifndef MY_FUNCTIONS
#define MY_FUNCTIONS
#include <string>
#include <vector>
using namespace std;
class my_functions
{
public:
	my_functions(void);
	~my_functions(void);
	string my_split(string in_string,char split_ch,int index);
};
#endif