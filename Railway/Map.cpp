#include "Map.h"
#include <fstream>

Map::Map()
{
	stations.clear();
	arcs.clear();
}

Map::~Map()
{
	stations.clear();
	arcs.clear();
}

void Map::addStation(Station newStation)
{
	stations.push_back(newStation);
}

void Map::deleteStation(Station delStation)
{
	int* temp = new int(-1);
	for (int index = 0; index < (int)stations.size(); index++)
		if (stations[index] == delStation)
		{
			*temp = index;
			break;
		}

	if (temp >= 0)
	{
		for (int index = int(*temp) + 1; index < (int)stations.size(); index++)
			swap(stations[index], stations[index - 1]);

		stations.pop_back();
	}

	delete temp;
}

void Map::addArc(Arc newArc)
{
	arcs.push_back(newArc);
}

void Map::deleteArc(Arc delArc)
{
	int* temp = new int(-1);
	for (int index = 0; index < (int)stations.size(); index++)
		if (arcs[index].getFirstStation().getName() == delArc.getFirstStation().getName() &&
			arcs[index].getSecondStation().getName() == delArc.getSecondStation().getName())
		{
			*temp = index;
			break;
		}

	if (temp >= 0)
	{
		for (int index = int(*temp) + 1; index < arcs.size(); index++)
			swap(arcs[index], arcs[index - 1]);

		arcs.pop_back();
	}

	delete temp;
}

void Map::scanMap()
{
	scanStations();
	scanArcs();

	cout << "Âגוהטעו טלÿ פאיכא ס ךאנעמי: ";
	
	cin >> fileName;
	ifstream map(fileName);

	string temp;
	while (getline(map, temp))
		mapX.push_back(temp);

	mapY = mapX;
	for (int i = 0; i < mapY.size(); ++i)
	{
		for (int j = 0; j < mapY[i].size(); ++j)
			if (mapY[i][j] == '1') mapY[i][j] = char(92);
			else if (mapY[i][j] >= '0' && mapY[i][j] <= '9') mapY[i][j] = '_';
	}
	mapZ = mapY;

	trueMap = mapY;
	for (int i = 0; i < trueMap.size(); ++i)
	{
		for (int j = 0; j < trueMap[i].size(); ++j)
			if (trueMap[i][j] == '_' || trueMap[i][j] == char(92) || trueMap[i][j] == '/' || trueMap[i][j] == '|') trueMap[i][j] = '1';
	}

	map.close();
}

vector <Station> Map::getStations()
{
	return stations;
}

vector <Arc> Map::getArcs()
{
	return arcs;
}

void Map::printMap()
{
	for (auto line : mapZ)
		cout << line << endl;
}

void Map::scanStations()
{
	scanResources();

	ifstream stations("stations.txt");

	stations >> numberOfStations;
	for (int index = 0; index < numberOfStations; ++index)
	{
		Station temp;
		stations >> temp.number >> temp.type;

		if (temp.type == 1)
		{
			stations >> temp.numberOfPassenger;
		}
		else
		{
			if (temp.type == 3)
			{
				stations >> temp.numberOfPassenger;;
			}

			int numberOfResource;
			stations >> numberOfResource;
			Resource tempRes = listOfResources[numberOfResource];
			stations >> tempRes.amount;
			temp.addNewResources(tempRes);
		}

		stations >> temp.name;
		this->stations.push_back(temp);
	}

	stations.close();
}

void Map::updateStation(Station newStation)
{
	for (int i = 0; i < stations.size(); ++i)
		if (stations[i].name == newStation.name)
		{
			stations[i] = newStation;
			break;
		}
}

void Map::scanArcs()
{
	ifstream arcs("arcs.txt");

	arcs >> numberOfArcs;
	for (int index = 0; index < numberOfArcs; ++index)
	{
		Arc temp;
		int first, second;

		arcs >> first >> second >> temp.distance;

		temp.firstStation = this->stations[first];
		temp.secondStation = this->stations[second];

		this->arcs.push_back(temp);
	}

	arcs.close();
}

void Map::scanResources()
{
	ifstream resources("resources.txt");

	resources >> numberOfResources;
	for (int index = 0; index < numberOfResources; ++index)
	{
		Resource temp;
		resources >> temp.name >> temp.rechargeRate;
		this->listOfResources.push_back(temp);
	}

	resources.close();
}
