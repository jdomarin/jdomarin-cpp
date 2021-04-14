#include <cstdlib> // atoi
#include <iostream> // cout, endl

using namespace std;

int isprime(int n)
{
	if(n < 2)
	{
		return 1;
	}
	for (int i = 2; i < n; i++)
	{
		if (n % i == 0)
		{
			return i;
		}
	}
	return 0;
}

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		int result = isprime(atoi(argv[1]));
		if (0 ==  result)
		{
			cout << "true" << endl;
		}
		else
		{
			cout << "false (multiple of " << result << ")" << endl;
		}
		return 0;
	}
	else if (argc == 3)
	{
		int start = atoi(argv[1]);
		int end = atoi(argv[2]);
		for (int i = start; i <= end; i++)
		{
			if (0 == isprime(i))
			{
				cout << i << " ";
			}
		}
		cout << endl;
		return 0;
	}
	else
	{
		cout << "Usage: isprime integer" << endl;
		cout << "       isprime start end" << endl;
		return 1;
	}
}
