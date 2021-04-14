#include <iostream>
#include <string>

using namespace std;

void permute(string, int);
string swap(string, int, int);

int main(int argc, char *argv[])
{
    if(argc != 2)
	cout << "Usage: " << argv[0] << " <string>" << endl;
    else
	permute(argv[1], 0);
    return 0;
}

void permute(string topermute, int place)
{
    if(topermute.length() - 1 == place)
	cout << topermute << endl;
    else
    {
	for(int nextchar = place; nextchar < topermute.length(); nextchar++)
	{
	    permute(swap(topermute, place, nextchar), place+1);
    	}
    }
}

string swap(string topermute, int x, int y)
{
    string newstring = topermute;
    newstring[x] = newstring[y];
    newstring[y] = topermute[x];//avoids temporary variable
    return newstring;
}
