#pragma once
#include "Map.h"
#include "Train.h"

class RailwayX
{
	vector<int> route;

	public:

		Map map;
		Train train;

		void setTrainOnTheStation(int numberOfStation);
		void goToTheStation(int numberOfStation);
		void scan();
		void stay();
		void setRoute();
		void genRes();
		void loading();
		void unloading();
		void print();
		void autoAction();
		void action();
		void start();
};

