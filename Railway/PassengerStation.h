#pragma once
#include <string>
#include <set>

using namespace std;

class PassengerStation
{
	public:

		PassengerStation();
		~PassengerStation();
			
		void setNumberOfPassenger(int number) { *numberOfPassenger = number; }
		int getNumberOfPassenger() { return *numberOfPassenger; }

	private:

		int *numberOfPassenger;
};

PassengerStation::PassengerStation()
{
	numberOfPassenger = new int(0);
}

PassengerStation::~PassengerStation()
{
	delete numberOfPassenger;
}