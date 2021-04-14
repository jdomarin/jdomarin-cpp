#include <string>
#include <iostream>

using namespace std;

string num_to_text[] = {"", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen" };
string tens_to_text[] = {"", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety" };

string power_to_text[] = {"", "thousand", "million", "billion" };

string padIfNeeded (string ans)
{
    if(ans == "")
	return "";
    return " " + ans;	
}

string translateHundred(int how_many)
{
    if(how_many < 20)
    {
	return num_to_text[how_many];
    }
    int tens = how_many / 10;
    int ones = how_many % 10;
    return tens_to_text[tens] + padIfNeeded(num_to_text[ones]);
}

string translateThousand(int how_many)
{
    if(how_many < 100)
    {
	return translateHundred(how_many);
    }
    else
    {
    	int hundreds = how_many / 100;
    	int hundred_chunk = how_many % 100;
    	return num_to_text[hundreds] + " hundred" + padIfNeeded(translateHundred(hundred_chunk));
    }
}

int main()
{
	int n;
	cin >> n;
	string number;
	bool is_negative = false;
	if(n < 0)
	{
		is_negative = true;
		n *= -1;
	}

	int chunk_count = 0;
	while(n > 0)
	{
		if(n % 1000 != 0)
		{
			number = translateThousand(n % 1000) + padIfNeeded(power_to_text[chunk_count]+ padIfNeeded(number) );
		}
		n /= 1000;
		chunk_count++;
	}
	if(number == "")
		number = "zero";
	if(is_negative)
		number = "minus " + number;
	cout << number << endl;

}	

