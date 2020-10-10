#include "Carriage.h";

Carriage::Carriage()
{
	lifeTime = 50;
	capacity = 50;
	type = 1;
}

Carriage::~Carriage()
{
	lifeTime = 0;
	capacity = 0;
}

void Carriage::setLifeTime(int newLifeTime)
{
	lifeTime = newLifeTime;
}

void Carriage::setCapacity(int newCapacity)
{
	capacity = newCapacity;
}

void Carriage::setType(int newType)
{
	type = newType;
}

void Carriage::addResources(Resource newResource)
{
	fullness += newResource.amount;

	bool key = false;
	for (int i = 0; i < resources.size(); ++i)
		if (resources[i].name == newResource.name)
		{
			resources[i].amount += newResource.amount;
			key = true;
			break;
		}

	if (!key) resources.push_back(newResource);
}

int Carriage::getLifeTime()
{
	return lifeTime;
}

int Carriage::getCapacity()
{
	return capacity;
}

int Carriage::getType()
{
	return type;
}

void Carriage::setCoordinates(point valueCoordinates)
{
	coordinates = valueCoordinates;
}

point Carriage::getCoodrinates()
{
	return coordinates;
}

void Carriage::addPassengers(int newNumberOfPassenger)
{
	fullness += newNumberOfPassenger;
}
