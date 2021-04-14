#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv)
{
    if(argc != 2)
    {
	cout << "Usage: " << argv[0] << " <filename>" << endl;
	return 1;
    }
    else
    {
	ifstream the_file (argv[1]);
	if(!the_file.is_open())
	{
	    cout << "Could not open file " << argv[1] << endl; 
	    return 2;
	}
	else 
	{
	    char x;
	    int file_size = 0;
	    while(the_file.get(x)) file_size++;
	    cout << file_size << " bytes." << endl;
	}
    }
    return 0;
}

