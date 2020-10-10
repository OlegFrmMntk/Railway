#pragma once
#include <vector>
#include "Resource.h"

using namespace std;

struct point
{
	int x, y;
};

class Carriage
{
	public:	

		Carriage();
		~Carriage();

		void setLifeTime(int newLifeTime);
		void setCapacity(int newCapacity);
		void setType(int newType);
		void addResources(Resource newResource);
		void addPassengers(int newNumberOfPassenger);
		int getLifeTime();
		int getCapacity();
		int getType();
		
		void setCoordinates(point valueCoordinates);
		point getCoodrinates();

	public:
		int lifeTime, capacity, type, fullness;
		point coordinates;
		vector<Resource> resources;
};
