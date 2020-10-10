#pragma once
#include <string>
#include <set>
#include <vector>
#include "Resource.h"

using namespace std;

class FreightStation
{
	public:

		FreightStation();
		~FreightStation();

		void addNewResources(Resource newResource) 
		{ 
			for (auto now : *resources)
				if (newResource.getName() == now.getName())
				{
					now.loadResource(newResource.getAmount());
					return;
				}

			resources->push_back(newResource);
		};

		void loadResources(string name, int amount) 
		{
			for (auto now : *resources)
				if (now.getName() == name)
				{
					now.loadResource(amount);
					break;
				}
		}

		int unloadResources(string name, int amount)
		{
			for (auto now : *resources)
				if (now.getName() == name)
				{
					return now.unloadResource(amount);
				}

			return 0;
		}

		vector <Resource> getResources() { return *resources; }

	protected:

		vector <Resource> *resources;
};

FreightStation::FreightStation()
{
	resources = new vector <Resource>;
}

FreightStation::~FreightStation()
{
	delete resources;
}