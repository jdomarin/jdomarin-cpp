#include <iostream>
#include <sstream>

using namespace std;

string convert(string number)
{
    string res = "";//the string to build, return and print out to the user

    if(number == "0" || number == "-0")//if the user entered 0
	res = "zero";

    //check number (as a string) length
    int len = number.length();
    int remaining_count;
    bool add_digit = false;
    bool is_zero = false; 
    
    //start looping on the string position
    for(int currPos = 0; currPos < len; currPos++) 
    {
	is_zero = (number[currPos] == '0');//current digit is a zero
	remaining_count = len - 1 - currPos;//number of digits left in number (left to right scanning)
	add_digit = (remaining_count % 3 != 1);//can add digit string if there are 3*n+1 digits left in number, with n>=0
	if(add_digit)
	{
	    switch(number[currPos])
	    {   //manage units
	    case '1':
	    	res += "one ";
	    	break;
	    case '2':
	    	res += "two ";
	    	break;
	    case '3':
	    	res += "three ";
	    	break;
	    case '4':
	    	res += "four ";
	    	break;
	    case '5':
	    	res += "five ";
	    	break;
	    case '6':
	    	res += "six ";
	    	break;
	    case '7':
	    	res += "seven ";
	    	break;
	    case '8':
	    	res += "eight ";
	    	break;
	    case '9':
	    	res += "nine ";
	    	break;
	    default:
	    	break;
	    }
        }
        if((remaining_count % 3 == 2) && !is_zero)//if there are 3*n+2 digits left in number, with n>=0
        {
    	    res += "hundred ";//append "hundred" since the current digit is the hundred digit
	}
    	else if((remaining_count == 3))
    	{
	    res += "thousand ";
    	}
    	else if((remaining_count == 6))
    	{
	    res += "million ";
    	}
    	else if((remaining_count == 9))
    	{
	    res += "billion ";
    	}
        else if(remaining_count % 3 == 1)
        {   //manage multiples of ten
	    switch(number[currPos])
	    {
		case '1':
		    switch(number[currPos+1])
		    {	//manage two-digit numbers below 20: fetch the value of next character
			case '0':
			    res += "ten ";
			    break;
			case '1':
			    res += "eleven ";
			    break;
			case '2':
			    res += "twelve ";
			    break;
			case '3':
			    res += "thirteen ";
			    break;
			case '4':
			    res += "fourteen ";
			    break;
			case '5':
			    res += "fifteen ";
			    break;
			case '6':
			    res += "sixteen ";
			    break;
			case '7':
			    res += "seventeen ";
			    break;
			case '8':
			    res += "eighteen ";
			    break;
			case '9':
			    res += "nineteen ";
			    break;
			default:
			    break;
		    }//end inner case '1'
		    currPos++;//skip next digit
		    break;
		case '2':
	    	    res += "twenty ";
		    break;
		case '3':
	    	    res += "thirty ";
	  	    break;
		case '4':
	    	    res += "forty ";
	    	    break;
		case '5':
	    	    res += "fifty ";
	    	    break;
		case '6':
	    	    res += "sixty ";
	    	    break;
		case '7':
	    	    res += "seventy ";
		    break;
		case '8':
	    	    res += "eighty ";
	    	    break;
		case '9':
	    	    res += "ninety ";
	    	    break;
		default:
	    	    break;
	    }//end switch(number[currPos])
        }//end if(remaining_count == 3*n+1, n>=0)
   }//end for loop
 
   return res;
}

int main(int argc, char *argv[])
{
    if(argc != 2) 
	cout << "Usage: " << argv[0] << " <signed_integer_to_convert>" << endl;
    else
    {
	istringstream iss(argv[1]);
	ostringstream oss;
	int tmp;
	iss >> tmp;//getting input string as integer
	if(tmp < 0)
	{
	    cout << "minus ";
	    tmp *= -1;//remove minus sign (make number positive)
	    oss << tmp;
	    cout << convert(oss.str());
	}
	else 
	    cout << convert(argv[1]);
	cout << endl;
    }
    return 0;
}
