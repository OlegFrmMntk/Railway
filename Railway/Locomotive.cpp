#include "Locomotive.h"

Locomotive::Locomotive()
{
	traction = 200;
	lifeTime = 50;
}

Locomotive::~Locomotive()
{
	traction = 0;
	lifeTime = 0;
}

void Locomotive::setTraction(int newTraction)
{
	traction = newTraction;
}

void Locomotive::setLifeTime(int newLifeTime)
{
	lifeTime = newLifeTime;
}

int Locomotive::getTraction()
{
	return traction;
}

int Locomotive::getLifeTime()
{
	return lifeTime;
}

void Locomotive::setCoordinates(point valueCoordinates)
{
	coordinates = valueCoordinates;
}

point Locomotive::getCoodrinates()
{
	return coordinates;
}
