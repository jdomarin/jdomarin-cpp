// factorial calculation
#include <iostream>
using namespace std;

long factorial(long n)
{
	if(n==0)
		return 1;
	else
		return n*factorial(n-1);
}

int main()
{
	long number;
	cout << "Please type a number: ";
	cin >> number;
	cout << number << "! = " << factorial(number);
	return 0;
}
