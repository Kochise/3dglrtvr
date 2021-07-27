#include "../cmd940/include/cmd940.h"
#include <iostream>
#include <cassert>

using namespace std;

int main()
{
	if (!cmd940_init("cmd940/bin/code940.bin")) {
		cout << "error initializing cmd940" << endl;
		return 1;
	}
	atexit(cmd940_deinit);

	// Syncronous call to fibonacci function
	int n = 10;
	cout << "fibonacci(" << n << ") = " << cmd940::fibonacci(n) << endl;

	// Call the asynchrous fibonacci version which internaly just calls the
	// normal fibonacci function on the 940.
	n = 25;
	cmd940::fibonacci_async(n);

	// ... do some work ...

	// Get and print the result
	cout << "fibonacci_async(" << n << ") = " << 
		cmd940::fibonacci_async_result() << endl;
		
	// ensure correct operation on the 940
	assert(cmd940::test1() == 0);
	assert(cmd940::test2() == 12345);
	assert(cmd940::test3() == 5);
	assert(cmd940::test4() == 12345);
}
