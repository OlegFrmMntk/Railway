#pragma once
#include "Resource.h"
#include <iostream>
#include <vector>
#include <string>


Resource::Resource()
{
	name = "Unnamed";
	amount = int(0);
	rechargeRate = int(1);
}

Resource::~Resource()
{

}

const bool operator <(const Resource& x, const Resource& y)
{
	return x.name < y.name;
}

void Resource::setNameResource(string newName)
{
	name = newName;
}

void Resource::setAmount(int newAmount)
{
	amount = newAmount;
}

void Resource::setRechargeRate(int newRate)
{
	rechargeRate = newRate;
}

string Resource::getNameResource()
{
	return name; 
}

int Resource::getAmount()
{
	return amount; 
}

int Resource::getRechargeRate() 
{
	return rechargeRate;
}

void Resource::replenishment()
{
	amount += rechargeRate;
}

void Resource::loadResource(int amt)
{
	amount += amt;
}

void Resource::unloadResource(int amt)
{
	if (amt > amount)
	{
		amt = amount;
		amount = 0;
	}
	else
	{
		amount -= amt;
	}
}