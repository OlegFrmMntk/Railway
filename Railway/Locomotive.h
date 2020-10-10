#pragma once
#include "Carriage.h"

class Locomotive
{
	public:

		Locomotive();
		~Locomotive();

		void setTraction(int newTraction);
		void setLifeTime(int newLifeTime);

		int getTraction();
		int getLifeTime();

		void setCoordinates(point valueCoordinates);
		point getCoodrinates();

	public:

		point coordinates;
		int traction, lifeTime;
};
