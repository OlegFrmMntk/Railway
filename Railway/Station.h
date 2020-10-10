#pragma once
#include <string>
#include <set>
#include <vector>
#include "Resource.h"

using namespace std;

class Station
{
public:

	Station();
	~Station();

	void setName(string newName);
	void setStationNumber(int newNumber);
	void setType(int newType);
	void setNumber(int newNumber);
	void setNumberOfPassenger(int number);
	void addNewResources(Resource newResource);
	void loadResources(string name, int amount);

	string getName();
	int getNumber();
	void unloadResources(string name, int amount);
	int getType();
	int getNumberOfPassenger();
	vector <Resource> getResources();

	friend const bool operator ==(const Station& x, const Station& y);
	friend const bool operator !=(const Station& x, const Station& y);
	friend const bool operator <(const Station& x, const Station& y);
	friend const bool operator >(const Station& x, const Station& y);
	friend const bool operator <=(const Station& x, const Station& y);
	friend const bool operator >=(const Station& x, const Station& y);

	public:
		string name;
		int number, type, numberOfPassenger;
		vector <Resource> resources;
};