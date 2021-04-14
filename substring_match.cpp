#include <iostream>

using namespace std;

bool is_substring(char *arg1, char* arg2)
{
    if(arg2.length() > arg1.length())
	return false;
    else if(arg1.length() == 0)
	return false;
    else
    {
	for(char* sub = *arg2[arg2.length() - 1]; sub = ""; sub--)
	{
	    return is_substring(arg1, sub);
    	}
}

int main(int argc, char **argv)
{
    if(argc != 3)
	cout << "Usage: " <<  argv[0] << " string_1 string_2 (returns true if string_2 is a substring of string_1, otherwise false))" << endl;
    else
    {
	cout << is_substring(argv[1], argv[2]) << endl;
    } 
}
