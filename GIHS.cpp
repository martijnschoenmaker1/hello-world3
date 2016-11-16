#include "stdafx.h"
#include "GIHS.h"
#include <iostream>
#include <fstream>
using namespace std;

GIHS::GIHS()
{
}

GIHS::GIHS(string input_name)
{
	char dum;
	// Open file
	ifstream in;
	in.open(input_name);

	// Read in nr of customers and facilities
	in >> nr_f;
	in >> nr_c;

	// Resize Capacity and cost vector for facilities
	Capacity.resize(nr_f);
	FCost.resize(nr_f);

	// Resize demand vector
	Demand.resize(nr_c);

	// Resize cost matrix
	C.resize(nr_c);
	for (int i = 0; i != nr_c; i++)
	{
		C.at(i).resize(nr_f);
	}

	// Read in Capacity and Opening costs for facilities
	for (int i = 0; i != nr_f; i++)
	{
		in >> Capacity.at(i);
		in >> FCost.at(i);
	}

	// Read in demand vector
	for (int i = 0; i != nr_c; i++)
	{
		in >> Demand.at(i);
		in >> dum;
	}

	// Read in cost matrix
	for (int i = 0; i != nr_c; i++)
	{
		for (int j = 0; j != nr_f; j++)
		{
			in >> C.at(i).at(j);
			in >> dum;
		}
	}

	// Close file
	in.close();
}

void GIHS::roundFacilityCost(int roundFactor)
{
	for (int i = 0; i != nr_f; i++)
	{
		FCost.at(i) = FCost.at(i) * roundFactor;
	}
}

int GIHS::JMS()
{
	// To store the budget of all cities in
	vector<int> BC;
	BC.resize(nr_c);

	// To store whether a city is connected and set all of them to not connected
	CC.resize(nr_c);
	for (int i = 0; i != nr_c; i++)
	{
		CC.at(i) = -1;
	}

	// Set all facilities to not connected
	Y.resize(nr_f);
	for (int i = 0; i != nr_f; i++)
	{
		Y.at(i) = false;
	}

	bool all_cities_connected = false;
	
	while (!all_cities_connected)
	{
		// For all cities that have not been connected, increase the budget
		for (int i = 0; i != nr_c; i++)
		{
			if (CC.at(i) == -1)
			{
				BC.at(i) = BC.at(i) + 1;
				int temp;
			}
		}

		// For all facilities that have not been connected, check whether you can connect them
		for (int i = 0; i != nr_f; i++)
		{
			int Tot_B = 0;
			for (int j = 0; j != nr_c; j++)
			{
				if (CC.at(j) == -1) // If city is not connected
				{
					if (BC.at(j) - C.at(j).at(i) > 0)
					{
						Tot_B += BC.at(j) - C.at(j).at(i);
					}
				}
				else // If city is connected
				{
					if (C.at(j).at(CC.at(j)) - C.at(j).at(i) > 0)
					{
						Tot_B += C.at(j).at(CC.at(j)) - C.at(j).at(i);
					}
				}
			}

			// If we can open facility i
			if (Tot_B >= FCost.at(i))
			{
				// Open facility i
				Y.at(i) = true;

				// And check which customers connect to it
				for (int j = 0; j != nr_c; j++)
				{
					if (CC.at(j) == -1) // If city is not connected
					{
						if (BC.at(j) - C.at(j).at(i) > 0) // And it contributes to facility i
						{
							CC.at(j) = i; // Connect it to facility i
						}
					}
					else // If city was already connected
					{
						if (C.at(j).at(CC.at(j)) - C.at(j).at(i) > 0) // But it is willing to contribute
						{
							CC.at(j) = i; // Connect it to facility i
						}
					}
				}
			}
			else
			{
				for (int j = 0; j != nr_c; j++) // For some city ...
				{
					if (CC.at(j) == -1 && Y.at(i) == true &&BC.at(j) == C.at(j).at(i)) // ... that is not connected, a facility that is already open and the city has budget is equal to connection cost
					{
						Y.at(i) = true; // Open facility ...
						CC.at(j) = i; // ... And connect it to customer
					}
				}
			}
		}

		// Check whether all cities are connected yet
		all_cities_connected = true;
		for (int i = 0; i != nr_c; i++)
		{
			if (CC.at(i) == -1)
			{
				all_cities_connected = false;
				break;
			}
		}
	}

	return 0;
}

void GIHS::PrintRoutes()
{
	cout << "Customer - Facility served by" << endl;
	for (int i = 0; i != nr_c; i++)
	{
		cout << i << " - " << CC.at(i) << endl;
	}
}

void GIHS::PrintAll()
{
	// Print Capacity and Opening costs for facilities
	cout << "i - Capacity - Cost" << endl;
	for (int i = 0; i != nr_f; i++)
	{
		cout << i << " - " << Capacity.at(i) << " - " << FCost.at(i) << endl;
	}
	cout << endl;

	// Print demand vector
	cout << "Demand vector" << endl;
	for (int i = 0; i != nr_c; i++)
	{
		cout << i << " - " << Demand.at(i) << endl;
	}
	cout << endl;

	// Print cost matrix
	cout << "Cost matrix:" << endl;
	for (int i = 0; i != nr_c; i++)
	{
		for (int j = 0; j != nr_f; j++)
		{
			cout << C.at(i).at(j) << " ";
		}
		cout << endl;
	}
}

void GIHS::PrintOpenFacilities()
{
	cout << "The open facilities are ";
	for (int i = 0; i != nr_f; i++)
	{
		if (Y.at(i))
		{
			cout << i << ", ";
		}
	}
	cout << endl;
}

int GIHS::TotalCost()
{
	int Total = 0;

	// Add facility costs
	for (int i = 0; i != nr_f; i++)
	{
		if (Y.at(i))
		{
			Total += FCost.at(i);
		}
	}

	// Add travell cost
	for (int i = 0; i != nr_c; i++)
	{
		Total += C.at(i).at(CC.at(i));
	}

	return Total;
}

GIHS::~GIHS()
{
}
