#pragma once
#include "Station.h"
#include "Locomotive.h"
#include "Carriage.h"
#include "Map.h"
#include <vector>

using namespace std;


class Train
{
	public:

		Train();
		~Train();

		void setLocomotive(Locomotive newLocomotive);
		Locomotive getLocomotive();
		void hookCarriage(Carriage newCarriage);
		Carriage unhookCarriage();
		vector <Carriage> getCarriages();
		void setStation(Station newStation);
		Station getStation();
		void scanTrain();
		void printTrain();
		void addPassenger(int count);
		void delPassenger(int count);
		int addResource(Resource resource);
		int delResource(Resource resource);
		vector <Resource> getResources(Map map);
		int getNumberOfPassenger();

	public:

		
		int numberOfCarriages;
		Locomotive locomotive;
		vector <Carriage> carriages;
		Station station;
		point coordinates;
};
