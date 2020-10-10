#pragma once
#include <string>
#include <set>
#include "Arc.h"
#include "Station.h"
#include "Resource.h"

using namespace std;

class Map
{
	public:

		Map();
		~Map();
	
		void updateStation(Station newStation);
		void addStation(Station newStation);
		void deleteStation(Station delStation);
		void addArc(Arc newArc);
		void deleteArc(Arc delArc);
		void scanMap();
		void printMap();
		void scanStations();
		void scanArcs();
		void scanResources();

		vector <Station> getStations();
		vector <Arc> getArcs();
		
		string fileName;
		vector <Station> stations;
		vector <Arc> arcs; 
		vector <Resource> listOfResources;

	public:

		int numberOfStations, numberOfArcs, numberOfResources;
		vector <string> mapX, mapY, mapZ, trueMap;
};
