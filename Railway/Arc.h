#pragma once
#include <string>
#include <set>
#include "Station.h"

using namespace std;

class Arc
{
	public:

		void setFirstStation(Station station) { firstStation = station; }
		void setSecondStation(Station station) { secondStation = station; }
		void setDistance(int newDistance) { distance = newDistance; }
		
		Station getFirstStation() { return firstStation; }
		Station getSecondStation() { return secondStation; }
		int getDistance() { return distance; }

	public:

		Station firstStation, secondStation;
		int distance;
};