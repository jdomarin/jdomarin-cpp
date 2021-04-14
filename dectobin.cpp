#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main(int argc, char *argv[])
{
	if(argc != 2) 
	{
	    cout << "Usage: " << argv[0] << " <number_in_decimal>" << endl;
	    return 1;
	}
	else
	{
	    string output = "";
	    int number, remainder;//number: inout string as integer, remainder: remainder of the division of number by 2
	    istringstream iss(argv[1]);
	    ostringstream oss;
	    iss >> number;//convert number to integer
	    if(number == 0)
	    {
		output = "0";
	    }
	    else
	    {
	    	while(number > 0)
	    	{	//divide number by 2 and concatenate remainders
	    		remainder = number % 2;
	    		number /= 2;
	   		oss << remainder;
	   	}
	    	output = oss.str();//retrieve concatenated remainders as a string
	    	output = string(output.rbegin(), output.rend());//reverse string since LSB and MSB are reversed
	    }
	    cout << output << endl;
	    return 0;
	}
}
