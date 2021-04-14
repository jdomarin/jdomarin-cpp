#include <iostream>

using namespace std;

long factorial(long n)
{
	if(n == 0) return 1;
	else return n * factorial(n-1);
}

int main()
{
	int num_zeros = 0;
	long number, fact;
	cout << "Enter a number: ";
	cin >> number;
	fact = factorial(number);
	while(fact % 10 == 0)
	{
		num_zeros++;
		fact /= 10;
	}
	cout << "There are " << num_zeros << " zeros in " << number << "!" << endl;
}

