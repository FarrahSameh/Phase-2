#include <iostream>
#include "Order.h"
#include "Chef.h"
#include "Table.h"
#include "Scooter.h"
#include "Restaurant.h"    

using namespace std;


int main()
{
	// ---- Testing LoadFromFile ----
	Restaurant R;
	string filename;
	cout << "\nEnter input file name: ";
	cin >> filename;
	R.LoadFromFile(filename);
	cout << "File loaded successfully!" << endl;
	R.Simulate();
	cout << "Simulate finished!" << endl;
	// ---- END ----

	return 0;
}