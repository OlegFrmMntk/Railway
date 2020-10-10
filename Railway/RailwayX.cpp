#include <iostream>
#include "RailwayX.h"
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <windows.h>

struct qPoint
{
	int x, y, ancestor;
};

void RailwayX::start()
{
	scan();

	setTrainOnTheStation(route[0]);

	cout << "\n__________________________________________________________\n\n";
	cout << "Продолжать работу в автоматическом режиме?" << endl;
	cout << "__________________________________________________________\n\n";
	cout << "1)Да" << endl;
	cout << "2)Нет";

	int number;
	while (true)
	{
		cout << "\n__________________________________________________________\n";
		cout << "\nВведите номер режима работы: ";
		cin >> number;

		if (number < 1 || number > 2)
		{
			cout << "\n__________________________________________________________\n";
			cout << " !!! Номер режима работы введен некорректно !!! " << endl;
		}
		else
		{
			cout << "\n__________________________________________________________\n\n";
			cout << " Режим работы выбран успешно." << endl;
			break;
		}
	}

	if (number == 1)
	{
		autoAction();

		while (true)
		{
			for (int i = 1; i < route.size(); ++i)
			{
				goToTheStation(route[i]);
				autoAction();
			}

			for (int i = route.size() - 2; i >= 0; --i)
			{
				goToTheStation(route[i]);
				autoAction();
			}
		}
	}
	else
	{
		action();

		while (true)
		{
			for (int i = 1; i < route.size(); ++i)
			{
				goToTheStation(route[i]);
				action();
			}

			for (int i = route.size() - 2; i >= 0; --i)
			{
				goToTheStation(route[i]);
				action();
			}
		}
	}
}

void RailwayX::scan()
{
	train.scanTrain();
	map.scanMap();
	setRoute();
}

void RailwayX::setRoute()
{
	int numberOfRoutes;
	ifstream routes("routes.txt");
	routes >> numberOfRoutes;

	vector<vector<int>> tempRoutes;
	for (int i = 1; i <= numberOfRoutes; ++i)
	{
		vector <int> temp;
		int amountOfStations;
		routes >> amountOfStations;

		for (int j = 0; j < amountOfStations; ++j)
		{
			int numberOfStation;
			routes >> numberOfStation;

			temp.push_back(numberOfStation);
		}
		tempRoutes.push_back(temp);
	}

	cout << "__________________________________________________________\n";
	cout << "\nПоезд может двигаться по следующим маршрутам: \n" << endl;
	for (int i = 0; i < tempRoutes.size(); ++i)
	{
		cout << "   " << i + 1 << " :: " << map.stations[tempRoutes[i][0]].name;
		for (int j = 1; j < tempRoutes[i].size(); ++j)
		{
			cout << " -> " << map.stations[tempRoutes[i][j]].name;
		}
		cout << ";" << endl;
	}
	

	while (true)
	{

		cout << "__________________________________________________________\n";
		cout << "\n Введите номер маршрута: ";

		int number;
		cin >> number;

		cout << "\n__________________________________________________________\n";

		if (number < 1 || number > tempRoutes.size())
		{
			cout << " !!! Номер маршрута введен некорректно !!! " << endl;
		}
		else
		{
			route = tempRoutes[int(number - 1)];
			cout << "\nМаршрут выбран." << endl;
			cout << "__________________________________________________________\n";
			break;
		}
	}	
}

void RailwayX::goToTheStation(int numberOfStation)
{
	train.station = map.stations[numberOfStation];

	bool mark[500][500];
	for (int i = 0; i < 500; ++i)
	{
		for (int j = 0; j < 500; ++j)
			mark[i][j] = false;
	}

	bool key = false;
	pair <int, int> stationCoordinates;
	for (int i = 0; i < map.mapX.size(); ++i)
	{
		for (int j = 0; j < map.mapX[i].size(); ++j)
			if (map.mapX[i][j] == char(numberOfStation + '0'))
			{
				stationCoordinates = { i, j };
				key = true;
				break;
			}
		if (key) break;
	}
	
	vector <qPoint> queue;

	int p1 = 0, p2 = 0;
	queue.push_back({ train.locomotive.coordinates.x, train.locomotive.coordinates.y, -1});
	mark[train.locomotive.coordinates.x][train.locomotive.coordinates.y] = true;
	const int step[8][2] = { {1, 0}, {0, 1}, {0, -1}, {-1, 0}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1} };

	key = false;
	while (p1 <= p2)
	{
		for (int index = 0; index < 8; ++index)
		{
			int x = queue[p1].x + step[index][0], y = queue[p1].y + step[index][1];

			if (x >= 0 && x < map.trueMap.size() && y >= 0 && y < map.trueMap[x].size() && mark[x][y] == false && map.trueMap[x][y] == '1')
			{
				queue.push_back({ x, y, p1 });
				mark[x][y] = true;
				p2++;
			}

			if (x == stationCoordinates.first && y == stationCoordinates.second)
			{
				key = true;
				break;
			}
		}
		if (key) break;

		p1++;
	}

	vector <pair<int, int>> way;
	int pos = queue.size() - 1;
	while (pos != -1)
	{
		way.push_back({ queue[pos].x, queue[pos].y });
		pos = queue[pos].ancestor;
	}
	reverse(way.begin(), way.end());
	
	int weight = 0;
	for (int i = 0; i < train.carriages.size(); ++i)
	{
		weight += train.carriages[i].fullness + 80;
	}
	// v^2 = 2r(f/m)
	// f = ma
	// a = f / m

	int speed = sqrt(double(double(2) * (double)way.size() * (double((double)train.locomotive.traction / (double)weight))));
	if (speed < 1) speed = 1;

	int length = train.carriages.size() + 1;
	for (int i = 0; i < way.size() - length; i += speed)
	{
		map.mapZ = map.mapY;
		point coordinates = train.locomotive.coordinates = { way[i].first, way[i].second };

		map.mapZ[coordinates.x][coordinates.y] = '#';

		int number = 0;
		for (int j = i + 1; j < i + length; ++j)
		{
			coordinates = train.carriages[number].coordinates = { way[j].first, way[j].second };
			map.mapZ[coordinates.x][coordinates.y] = '#';
			number++;
		}

		system("cls");
		map.printMap();
	}

	for (int i = way.size() - length - 1; i < way.size() - length; ++i)
	{
		genRes();
		map.mapZ = map.mapY;
		point coordinates = train.locomotive.coordinates = { way[i].first, way[i].second };

		map.mapZ[coordinates.x][coordinates.y] = '#';

		int number = 0;
		for (int j = i + 1; j < i + length; ++j)
		{
			coordinates = train.carriages[number].coordinates = { way[j].first, way[j].second };
			map.mapZ[coordinates.x][coordinates.y] = '#';
			number++;
		}

		system("cls");
		map.printMap();
	}

	train.station = map.stations[train.station.number];
}

void RailwayX::genRes()
{
	for (int i = 0; i < map.stations.size(); ++i)
		if (map.stations[i].type != 1) map.stations[i].resources[0].amount += map.stations[i].resources[0].rechargeRate;
}

void RailwayX::loading()
{
	vector <Resource> temp = train.station.getResources();
	vector <Resource> tempResources = map.listOfResources;
	
	cout << "\n\tВыбрана загрузка." << endl;

	if (train.station.getType() == 1 || train.station.getType() == 3)
	{
		while (true)
		{

			cout << "__________________________________________________________\n";
			cout << " \n Введите количество пассажиров: ";

			int* amountOfPassenger;
			amountOfPassenger = new int;
			cin >> *amountOfPassenger;

			if (*amountOfPassenger < 0 || *amountOfPassenger > train.station.numberOfPassenger)
			{
				cout << "\n !!! Количество пассажиров введено некорректно !!! \n" << endl;
			}
			else
			{
				train.addPassenger(*amountOfPassenger);
				cout << " Пассажиры успешно завершили посадку." << endl;
				break;
			}
		}
	}

	if (train.station.getType() == 2 || train.station.getType() == 3)
	{
		int* numberOfResource;
		int* amountOfResource;

		while (true)
		{

			cout << "__________________________________________________________\n" << endl;
			cout << " Введите номер ресурса, который хотите загрузить: ";

			numberOfResource = new int;
			cin >> *numberOfResource;

			if (*numberOfResource < 1 || *numberOfResource > temp.size())
			{
				cout << "\n\t!!! Некорректный номер ресурса !!!\n" << endl;
				delete numberOfResource;
			}
			else break;
		}

		while (true)
		{
			cout << " Введите количество ресурса: ";

			amountOfResource = new int;
			cin >> *amountOfResource;

			if (*amountOfResource < 0) cout << "\n\t!!! Некорректное количесво ресурса !!!\n" << endl;
			else if (*amountOfResource > temp[*numberOfResource - 1].amount)
			{
				cout << "\n\t!!! На складе недостаточно ресурсов !!!\n" << endl;
			}
			else
			{
				Resource* t;
				t = new Resource;
				*t = temp[*numberOfResource - 1];
				t->amount = *amountOfResource;
				*amountOfResource = train.addResource(*t);

				delete t;
				break;
			}


		}
		
		delete numberOfResource;
		delete amountOfResource;
	}
}

void RailwayX::unloading()
{
	vector <Resource> temp = train.station.getResources();
	
	cout << "\n\tВыбрана разгрузка." << endl;

	if (train.station.getType() == 1 || train.station.getType() == 3)
	{
		while (true)
		{

			cout << "__________________________________________________________\n";
			cout << " \n Введите количество пассажиров: ";

			int* amountOfPassenger;
			amountOfPassenger = new int;
			cin >> *amountOfPassenger;

			if (*amountOfPassenger < 0 || *amountOfPassenger > train.getNumberOfPassenger())
			{
				cout << "\n !!! Количество пассажиров введено некорректно !!! \n" << endl;
			}
			else
			{
				train.delPassenger(*amountOfPassenger);
				cout << " Пассажиры успешно сошли с поезда." << endl;
				break;
			}
		}
	}

	if (train.station.getType() == 2 || train.station.getType() == 3)
	{

		vector <Resource> tempResources = train.getResources(map);

		int* numberOfResource;
		int* amountOfResource;

		while (true)
		{

			cout << "__________________________________________________________\n" << endl;
			cout << " Введите номер ресурса, который хотите разгрузить: ";

			numberOfResource = new int;
			cin >> *numberOfResource;

			if (*numberOfResource < 1 || *numberOfResource > tempResources.size())
			{
				cout << "\n\t!!! Некорректный номер ресурса !!!\n" << endl;
				delete numberOfResource;
			}
			else break;
		}

		while (true)
		{
			cout << " Введите количество ресурса: ";

			amountOfResource = new int;
			cin >> *amountOfResource;

			if (*amountOfResource < 0) cout << "\n\t!!! Некорректное количесво ресурса !!!\n" << endl;
			else
			{
				Resource* t;
				t = new Resource;
				*t = tempResources[*numberOfResource - 1];
				t->amount = *amountOfResource;
				*amountOfResource = train.delResource(*t);
				cout << " Было разгружено " << *amountOfResource << " единиц ресурса\n";
				cout << "__________________________________________________________\n" << endl;

				delete t;
				break;
			}


		}
		
		delete numberOfResource;
		delete amountOfResource;
	}
}

void RailwayX::stay()
{
	cout << "\n\tВыбрана стоянка временная стоянка." << endl << " Введите время стоянки(в часах): ";
	int time;
	cin >> time;
	Sleep(time * 1000);
}

void RailwayX::setTrainOnTheStation(int numberOfStation)
{
	train.station = map.stations[route[0]];

	bool mark[500][500];
	for (int i = 0; i < 500; ++i)
	{
		for (int j = 0; j < 500; ++j)
			mark[i][j] = false;
	}

	bool key = false;
	pair <int, int> stationCoordinates;
	for (int i = 0; i < map.mapX.size(); ++i)
	{
		for (int j = 0; j < map.mapX[i].size(); ++j)
			if (map.mapX[i][j] == char(numberOfStation + '0'))
			{
				stationCoordinates = { i, j };
				key = true;
				break;
			}
		if (key) break;
	}
	
	vector <pair<int, int>> queue;

	int p1 = 0, p2 = 0;
	queue.push_back({ stationCoordinates.first, stationCoordinates.second});
	mark[stationCoordinates.first][stationCoordinates.second] = true;
	const int step[8][2] = { {1, 0}, {0, 1}, {0, -1}, {-1, 0}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1} };

	key = false;
	while (p1 <= p2)
	{
		for (int index = 0; index < 8; ++index)
		{
			int x = queue[p1].first + step[index][0], y = queue[p1].second + step[index][1];

			if (x >= 0 && x < map.trueMap.size() && y >= 0 && y < map.trueMap[x].size() && mark[x][y] == false && map.trueMap[x][y] == '1')
			{
				queue.push_back({ x, y });
				mark[x][y] = true;
				p2++;
			}

			if (train.numberOfCarriages == p2)
			{
				key = true;
				break;
			}
		}
		if (key) break;

		p1++;
	}
	sort(queue.begin(), queue.end());

	train.locomotive.setCoordinates({ queue[0].first, queue[0].second });
	for (int i = 1; i <= p2; ++i)
		train.carriages[i - 1].setCoordinates({ queue[i].first, queue[i].second });

	train.printTrain();
}

void RailwayX::print()
{
	map.mapZ = map.mapY;
	point coordinates = train.locomotive.getCoodrinates();
	map.mapZ[coordinates.x][coordinates.y] = '#';

	for (int i = 0; i < train.numberOfCarriages; ++i)
	{
		coordinates = train.carriages[i].getCoodrinates();
		map.mapZ[coordinates.x][coordinates.y] = '#';
	}

	system("cls");
	map.printMap();
}

void RailwayX::action()
{

	cout << "__________________________________________________________\n";
	cout << "\nСостав прибыл на ";

	switch (train.station.getType())
	{
		case 1:
			cout << "пассажирскую";
			break;

		case 2:
			cout << "товарную";
			break;

		case 3:
			cout << "пассажирско-товарную";
			break;

		default:
			break;
	}
	
	cout << " станцию " << train.station.getName() << ".\n" << endl;

	vector <Resource> temp = train.station.getResources();
	switch (train.station.getType())
	{
	case 1:
		cout << "Ожидающих пассажиров на станции: " << train.station.numberOfPassenger << ".\n";
		break;

	case 2:
		cout << "\nВ наличии следующие ресурсы: \n\n";

		for (int i = 0; i < temp.size(); ++i)
		{
			cout << "   " << i + 1 << ":: Название: " << temp[i].getNameResource() << "\n       Количество: " << temp[i].amount << endl << endl;
		}
		break;

	case 3:
		cout << "Ожидающих пассажиров на станции: " << train.station.numberOfPassenger << ".\n";
		cout << "\nВ наличии следующие ресурсы: \n\n";
		for (int i = 0; i < temp.size(); ++i)
		{
			cout << "   " << i + 1 << ":: Название: " << temp[i].getNameResource() << "\n       Количество: " << temp[i].amount << endl << endl;
		}
		break;

	default:
		break;
	}

	cout << "__________________________________________________________\n";

	vector <Carriage> tempCarriages = train.getCarriages();
	vector <Resource> tempResources = train.getResources(map);
	
	int numberOfPassenger = 0;
	for (int i = 0; i < tempCarriages.size(); ++i)
	 if (tempCarriages[i].type == 1) numberOfPassenger += tempCarriages[i].fullness;
		

	cout << "\nВ поезде находится " << numberOfPassenger << " пассажиров\n" << endl;
	cout << "В составе следующие ресурсы: \n\n";
	for (int i = 0; i < tempResources.size(); ++i)
	{
		cout << "   " << i + 1 << ":: Название: " << tempResources[i].getNameResource() << "\n       Количество: " << tempResources[i].amount << endl << endl;
	}

	cout << "__________________________________________________________\n";
	cout << "\nВыберите действие: " << endl;
	cout << "\n   1)Загрузка.\n   2)Разгрузка.\n   3)Транзит.\n   4)Временная стоянка." << endl;
	cout << "\nВведите номер действия: ";
 
	int number;
	cin >> number;

	cout << "\n__________________________________________________________\n";

	bool key = false;
	switch (number)
	{
		case 1:
			loading();
			break;

		case 2:
			unloading();
			break;

		case 3:
			cout << "\n\tВыбран транзит.\n" << endl;
			break;

		case 4:
			stay();
			break;

		default:
			break;
	}

	map.updateStation(train.station);
}

void RailwayX::autoAction()
{

	cout << "__________________________________________________________\n";
	cout << "\nСостав прибыл на ";

	switch (train.station.getType())
	{
		case 1:
			cout << "пассажирскую";
			break;

		case 2:
			cout << "товарную";
			break;

		case 3:
			cout << "пассажирско-товарную";
			break;

		default:
			break;
	}
	
	cout << " станцию " << train.station.getName() << ".\n" << endl;

	vector <Resource> temp = train.station.getResources();
	switch (train.station.getType())
	{
	case 1:
		cout << "Ожидающих пассажиров на станции: " << train.station.numberOfPassenger << ".\n";
		break;

	case 2:
		cout << "\nВ наличии следующие ресурсы: \n\n";

		for (int i = 0; i < temp.size(); ++i)
		{
			cout << "   " << i + 1 << ":: Название: " << temp[i].getNameResource() << "\n       Количество: " << temp[i].amount << endl << endl;
		}
		break;

	case 3:
		cout << "Ожидающих пассажиров на станции: " << train.station.numberOfPassenger << ".\n";
		cout << "\nВ наличии следующие ресурсы: \n\n";
		for (int i = 0; i < temp.size(); ++i)
		{
			cout << "   " << i + 1 << ":: Название: " << temp[i].getNameResource() << "\n       Количество: " << temp[i].amount << endl << endl;
		}
		break;

	default:
		break;
	}

	cout << "__________________________________________________________\n";

	vector <Carriage> tempCarriages = train.getCarriages();
	vector <Resource> tempResources = train.getResources(map);
	
	int numberOfPassenger = 0;
	for (int i = 0; i < tempCarriages.size(); ++i)
	 if (tempCarriages[i].type == 1) numberOfPassenger += tempCarriages[i].fullness;
		

	cout << "\nВ поезде находится " << numberOfPassenger << " пассажиров\n" << endl;
	cout << "В составе следующие ресурсы: \n\n";
	for (int i = 0; i < tempResources.size(); ++i)
	{
		cout << "   " << i + 1 << ":: Название: " << tempResources[i].getNameResource() << "\n       Количество: " << tempResources[i].amount << endl << endl;
	}

	cout << "__________________________________________________________\n";
	cout << "\nВыберите действие: " << endl;
	cout << "\n   1)Загрузка.\n   2)Разгрузка.\n   3)Транзит.\n   4)Временная стоянка." << endl;
	cout << "\nВведите номер действия: ";
 
	int number = rand() % 4 + 1;
	cout << number << endl;

	cout << "\n__________________________________________________________\n";

//	vector <Resource> temp = train.station.getResources();
// 	vector <Resource> tempResources = map.listOfResources;

	bool key = false;
	switch (number)
	{
		case 1:
			
			cout << "\n\tВыбрана загрузка." << endl;

			if (train.station.getType() == 1 || train.station.getType() == 3)
			{
				while (true)
				{

					cout << "__________________________________________________________\n";
					cout << " \n Введите количество пассажиров: ";

					int* amountOfPassenger;
					amountOfPassenger = new int;
					if (train.station.numberOfPassenger > 0) *amountOfPassenger = (rand() * rand()) % train.station.numberOfPassenger;
					else *amountOfPassenger = 0;

					cout << *amountOfPassenger << endl;

					if (*amountOfPassenger < 0 || *amountOfPassenger > train.station.numberOfPassenger)
					{
						cout << "\n !!! Количество пассажиров введено некорректно !!! \n" << endl;
					}
					else
					{
						train.addPassenger(*amountOfPassenger);
						cout << " Пассажиры успешно завершили посадку." << endl;
						break;
					}
				}
			}

			if (train.station.getType() == 2 || train.station.getType() == 3)
			{
				int* numberOfResource;
				int* amountOfResource;

				while (true)
				{

					cout << "__________________________________________________________\n" << endl;
					cout << " Введите номер ресурса, который хотите загрузить: ";

					numberOfResource = new int;
					if (temp.size() > 0) *numberOfResource = (rand() * rand()) % temp.size();
					else *numberOfResource = 0;
					cout << *numberOfResource + 1 << endl;

					if (*numberOfResource < 0 || *numberOfResource > temp.size())
					{
						cout << "\n\t!!! Некорректный номер ресурса !!!\n" << endl;
						delete numberOfResource;
					}
					else break;
				}

				while (true)
				{
					cout << " Введите количество ресурса: ";

					amountOfResource = new int;
					if (temp[*numberOfResource].amount > 0) *amountOfResource = (rand() * rand()) % temp[*numberOfResource].amount;
					else *amountOfResource = 0;

					cout << *amountOfResource;

					if (*amountOfResource < 0) cout << "\n\t!!! Некорректное количесво ресурса !!!\n" << endl;
					else if (*amountOfResource > temp[*numberOfResource].amount)
					{
						cout << "\n\t!!! На складе недостаточно ресурсов !!!\n" << endl;
					}
					else
					{
						Resource* t;
						t = new Resource;
						*t = temp[*numberOfResource];
						t->amount = *amountOfResource;
						*amountOfResource = train.addResource(*t);

						delete t;
						break;
					}


				}

				delete numberOfResource;
				delete amountOfResource;
			}
			Sleep(2000);
			break;

		case 2:
		//	vector <Resource> temp = train.station.getResources();

			cout << "\n\tВыбрана разгрузка." << endl;

			if (train.station.getType() == 1 || train.station.getType() == 3)
			{
				while (true)
				{

					cout << "__________________________________________________________\n";
					cout << " \n Введите количество пассажиров: ";

					int* amountOfPassenger;
					amountOfPassenger = new int;

					if (train.getNumberOfPassenger() > 0) *amountOfPassenger = rand() % train.getNumberOfPassenger();
					else *amountOfPassenger = 0;
					cout << *amountOfPassenger << endl;

					if (*amountOfPassenger < 0 || *amountOfPassenger > train.getNumberOfPassenger())
					{
						cout << "\n !!! Количество пассажиров введено некорректно !!! \n" << endl;
					}
					else
					{
						train.delPassenger(*amountOfPassenger);
						cout << " Пассажиры успешно сошли с поезда." << endl;
						break;
					}
				}
			}

			if (train.station.getType() == 2 || train.station.getType() == 3)
			{

				//vector <Resource> tempResources = train.getResources(map);

				int* numberOfResource;
				int* amountOfResource;

				while (true)
				{

					cout << "__________________________________________________________\n" << endl;
					cout << " Введите номер ресурса, который хотите разгрузить: ";

					numberOfResource = new int;
					if (tempResources.size() > 0) *numberOfResource = (rand() * rand()) % tempResources.size();
					else *numberOfResource = 0;

					cout << *numberOfResource + 1 << endl;

					if (*numberOfResource < 0 || *numberOfResource > tempResources.size())
					{
						cout << "\n\t!!! Некорректный номер ресурса !!!\n" << endl;
						delete numberOfResource;
					}
					else break;
				}

				while (true)
				{
					cout << " Введите количество ресурса: ";

					amountOfResource = new int;
					*amountOfResource = (rand() * rand()) % 40;
					cout << *amountOfResource << endl;

					if (*amountOfResource < 0) cout << "\n\t!!! Некорректное количесво ресурса !!!\n" << endl;
					else
					{
						Resource* t;
						t = new Resource;
						*t = tempResources[*numberOfResource];
						t->amount = *amountOfResource;
						*amountOfResource = train.delResource(*t);
						cout << " Было разгружено " << *amountOfResource << " единиц ресурса\n";
						cout << "__________________________________________________________\n" << endl;

						delete t;
						break;
					}


				}

				delete numberOfResource;
				delete amountOfResource;
			}
			Sleep(2000);
			break;

		case 3:
			cout << "\n\tВыбран транзит.\n" << endl;
			break;

		case 4:
			cout << "\n\tВыбрана стоянка временная стоянка." << endl;
			Sleep((rand() % 10) * 1000);
			break;

		default:
			break;
	}

	map.updateStation(train.station);
}
