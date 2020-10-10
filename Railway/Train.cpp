#include "Train.h"
#include <fstream>
#include <algorithm>

Train::Train()
{
	carriages.clear();
}

Train::~Train()
{
	carriages.clear();
}

void Train::setLocomotive(Locomotive newLocomotive)
{
	locomotive = newLocomotive;
}

Locomotive Train::getLocomotive()
{
	return locomotive;
}

vector <Carriage> Train::getCarriages()
{
	return carriages;
}

void Train::hookCarriage(Carriage newCarriage)
{
	carriages.push_back(newCarriage);
}

Carriage Train::unhookCarriage()
{
	Carriage temp = carriages.back();

	carriages.pop_back();

	return temp;
}

void Train::setStation(Station newStation)
{
	station = newStation;
}

Station Train::getStation()
{
	return station;
}

void Train::scanTrain()
{
	ifstream locomotives("locomotive.txt");

	locomotives >> this->locomotive.traction >> this->locomotive.lifeTime;

	locomotives.close();

	ifstream carriages("carriages.txt");

	carriages >> numberOfCarriages;
	for (int index = 0; index < numberOfCarriages; ++index)
	{
		Carriage temp;
		carriages >> temp.type >> temp.lifeTime >> temp.capacity;

		this->carriages.push_back(temp);
	}

	carriages.close();
}

void Train::printTrain()
{
	cout << "\nВыбран следующий локомотив: \n\n";
	cout << "    " << 0 << ":: Тяговая сила = " << locomotive.traction << "; \n\tСрок службы = " << locomotive.lifeTime << ";" << endl << endl;

	cout << "В составе следующие вагоны:\n" << endl;
	for (int index = 0; index < numberOfCarriages; ++index)
	{
		cout << "    " << index + 1 << ":: Вид вагона = ";

		switch (this->carriages[index].type)
		{
		case 1:
			cout << "Пассажирский;" << endl;
			break;
		case 2:
			cout << "Товарный;" << endl;
		default:
			break;
		}

		cout << "\n\tВместительность = " << this->carriages[index].capacity << "; \n\tСрок Службы = " << this->carriages[index].lifeTime << ";" << endl << endl;
	}
}

int Train::addResource(Resource resource)
{
	while (resource.amount)
	{
		for (int i = 0; i < carriages.size(); ++i)
			if (carriages[i].type == 2)
			{
				int temp = min(carriages[i].capacity - carriages[i].fullness, resource.amount);
				Resource tempRes = resource;
				tempRes.amount = temp;

				for (int j = 0; j < station.resources.size(); ++j)
					if (station.resources[j].name == resource.name)
					{
						station.resources[j].amount -= min(temp, station.resources[j].amount);
						break;
					}

				resource.amount -= temp;
				carriages[i].addResources(tempRes);

				if (!resource.amount) break;
			}

		break;
	}

	return resource.amount;
}

int Train::delResource(Resource resource)
{

	int amount = 0;
	for (int i = 0; i < carriages.size(); ++i)
		if (carriages[i].type == 2)
		{
			for (int j = 0; j < carriages[i].resources.size(); ++j)
				if (carriages[i].resources[j].name == resource.name)
				{
					int temp = min(carriages[i].resources[j].amount, resource.amount);
					resource.amount -= temp;
					carriages[i].resources[j].amount -= temp;
					amount += temp;
					break;
				}

			if (!resource.amount) break;
		}

	resource.amount = amount;
	station.addNewResources(resource);

	return amount;
}

void Train::addPassenger(int count)
{
	while (count)
	{
		for (int i = 0; i < carriages.size(); ++i)
			if (carriages[i].type == 1)
			{
				int temp = min(count, carriages[i].capacity - carriages[i].fullness);
				carriages[i].fullness += temp;
				station.numberOfPassenger -= temp;
				count -= temp;
			}

		break;
	}
}

void Train::delPassenger(int count)
{
	while (count)
	{
		for (int i = 0; i < carriages.size(); ++i)
			if (carriages[i].type == 1)
			{
				int temp = min(count, carriages[i].fullness);
				carriages[i].fullness -= temp;
				station.numberOfPassenger += temp;
				count -= temp;
			}
		break;
	}
}

int Train::getNumberOfPassenger()
{
	int answer = 0;
	for (int i = 0; i < carriages.size(); ++i)
		if (carriages[i].type == 1) answer += carriages[i].fullness;
	return answer;
}

vector <Resource> Train::getResources(Map map)
{
	vector <Resource> tempResources = map.listOfResources;

	for (int i = 0; i < tempResources.size(); ++i)
		tempResources[i].amount = 0;

	int numberOfPassenger = 0;
	for (int i = 0; i < carriages.size(); ++i)
	{
		if (carriages[i].type == 1) numberOfPassenger += carriages[i].fullness;
		else
		{
			for (int j = 0; j < tempResources.size(); ++j)
			{
				for (int q = 0; q < carriages[i].resources.size(); ++q)
					if (carriages[i].resources[q].name == tempResources[j].name) tempResources[j].amount += carriages[i].resources[q].amount;
			}
		}
	}

	return tempResources;
}
