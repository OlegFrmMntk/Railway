#pragma once
#include <iostream>
#include <string>

using namespace std;

class Resource
{
	public:

		Resource();
		~Resource();

		void setNameResource(string newName);
		void setAmount(int newAmount);
		void setRechargeRate(int newRate);

		string getNameResource();
		int getAmount();
		int getRechargeRate();
		
		void replenishment();

		void loadResource(int amt);
		void unloadResource(int amt);

		friend const bool operator <(const Resource& x, const Resource& y);

	public:

		string name;
		int amount, rechargeRate;
};