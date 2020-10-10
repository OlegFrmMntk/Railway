#pragma once
#include "Station.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

Station::Station()
{
	name = string("Unnamed");
	number = 0;
	type = 1;
	numberOfPassenger = int(0);
}

Station::~Station()
{

}

const bool operator ==(const Station& x, const Station& y)
{
	return x.name == y.name && x.number == y.number && x.type == y.type && x.numberOfPassenger == y.numberOfPassenger;
}

const bool operator !=(const Station& x, const Station& y)
{
	return !(x.name == y.name && x.number == y.number && x.type == y.type && x.numberOfPassenger == y.numberOfPassenger);
}

const bool operator <(const Station& x, const Station& y)
{
	return x.number < y.number;
}

const bool operator >(const Station& x, const Station& y)
{
	return x.number > y.number;
}

const bool operator <=(const Station& x, const Station& y)
{
	return x.number <= y.number;
}

const bool operator >=(const Station& x, const Station& y)
{
	return x.number <= y.number;
}

void Station::setName(string newName)
{
	this->name = newName;
}

void Station::setStationNumber(int newNumber)
{
	this->number = newNumber;
}

void Station::setType(int newType)
{ 
	this->type = newType;
}

void Station::setNumberOfPassenger(int number)
{
	this->numberOfPassenger = number;
}

void Station::setNumber(int newNumber)
{
	this->number = newNumber;
}

string Station::getName()
{
	return this->name;
}

int Station::getNumber()
{
	return this->number;
}

int Station::getType()
{ 
	return this->type;
}

int Station::getNumberOfPassenger() 
{
	return this->numberOfPassenger;
}

void Station::addNewResources(Resource newResource)
{
	for (int i = 0; i < resources.size(); ++i)
		if (newResource.name == resources[i].name)
		{
			resources[i].amount += newResource.amount;
			return;
		}

	resources.push_back(newResource);
};

void Station::loadResources(string name, int amount)
{
	for (auto now : resources)
		if (now.getNameResource() == name)
		{
			now.loadResource(amount);
			break;
		}
}

void Station::unloadResources(string name, int amount)
{
	for (auto now : resources)
		if (now.getNameResource() == name)
		{
			now.unloadResource(amount);
		}

}

vector <Resource> Station::getResources() 
{
	return this->resources;
} 
