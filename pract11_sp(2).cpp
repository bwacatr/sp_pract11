// pract11_sp(2).cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <windows.h>
#include <fstream>
#include <conio.h>
#include <random>

using namespace std;


class Robot
{
	int energy = 60;
	int score = 0;
	string state = "working";
	int id = 0;
	



public:

	int* testNums;

	mt19937 mt;

	void TestSelection(int num)
	{
		switch (num)
		{
		case 1:
			ObstacleTest();
			break;
		case 2:
			SprintTest();
			break;
		case 3:
			Charge();
			break;
		case 4:
			Marathon();
			break;
		default:
			break;
		}
	}

	void ObstacleTest()
	{
		cout << "\n\nРобот" << id << " пытается проехать через препятствие\n" << endl;

		if (energy >= 10)
		{


			Sleep(2000);

			cout << "\n\nРобот" << id << " успешно преодолел препятствие\n" << endl;

			score += 15;
			energy -= 10;

			Sleep(1000);

			cout << "\n\nРобот" << id << " заработал 15 очков и потерял 10 единиц энергии.\n" << "Очки робота: " << score << "\nЭнергия робота: " << energy << endl;
		}
		else
		{
			Sleep(2000);
			cout << "\n\nРоботу" << id << " не хватило заряда. Испытание не пройдено\n" << endl;
			Sleep(1000);
		}



	}

	void SprintTest()
	{
		cout << "\n\nРобот" << id << " пытается использовать ускорение во время спринта\n" << endl;

		if (energy >= 20)
		{


			Sleep(2000);

			cout << "\n\nРобот" << id << " успешно прошел спринт\n" << endl;

			score += 30;
			energy -= 20;

			Sleep(2000);

			cout << "\n\nРобот" << id << " заработал 30 очков и потерял 20 единиц энергии.\n" << "Очки робота: " << score << "\nЭнергия робота: " << energy << endl;
		}
		else
		{
			Sleep(2000);
			cout << "\n\nРоботу" << id << " не хватило заряда. Испытание не пройдено\n" << endl;
			Sleep(2000);
		}



	}

	void Charge()
	{
		
		cout << "\n\nРобот" << id << " заряжается\n" << endl;

		int seconds = (mt() % 5 +1) * 1000;

		int energyGain = seconds / 100;


		energy += energyGain;

		if (energy > 100)
		{
			energy = 100;
		}

		Sleep(seconds);

		cout << "\n\nРобот" << id << " зарядился на " << energyGain << " Очки робота: " << score << "\nЭнергия робота: " << energy << endl;
	}

	void Marathon()
	{
		cout << "\n\nРобот" << id << " пытается использовать ускорение во время марафона\n" << endl;

		if (energy >= 60)
		{


			Sleep(8000);

			cout << "\n\nРобот" << id << " успешно прошел марафон\n" << endl;

			score += 55;
			energy -= 60;

			Sleep(2000);

			cout << "\n\nРобот" << id << " заработал 55 очков и потерял 60 единиц энергии.\n" << "Очки робота: " << score << "\nЭнергия робота: " << energy << endl;
		}
		else
		{
			Sleep(2000);
			cout << "\n\nРоботу" << id << " не хватило заряда. Испытание не пройдено\n" << endl;
			Sleep(8000);
		}
	}

	void Intermission()
	{
		
		int seconds = mt() % 4;
		seconds *= 1000;

		cout << "\n\nЗадержка между испытаниями: " << seconds / 1000 << " секунды\n" << endl;
		Sleep(seconds);
	}

	int GetId()
	{
		return id;
	}

	void SetId(int input)
	{
		if (input >= 0)
		{
			id = input;
		}
	}

	int GetEnergy()
	{
		return energy;
	}

	void SetEnergy(int input)
	{
		if (input >= 0)
		{
			energy = input;
		}
	}

	int GetScore()
	{
		return score;
	}

	void SetScore(int input)
	{
		if (input >= 0)
		{
			score = input;
		}
	}

	string GetState()
	{
		return state;
	}

	void SetState(int input) // 1 = w = working, 2 = h = halted, 3 = c = completed tests
	{
		switch (input)
		{
		case 1:
		{
			state = "working";
			break;
		}
		case 2:
		{
			state = "halted";
			break;

		}
		case 3:
		{
			state = "completed";
			break;
		}
		default:


			break;
		}
	}






};

struct Operator
{
	HANDLE* threads;
	Robot* robots;
	int robotAmount;
};

DWORD WINAPI StartTest(LPVOID lpParam)
{
	Robot* robot = static_cast<Robot*>(lpParam);

	for (int i = 0; i < sizeof(robot->testNums); i++)
	{
		int num = robot->testNums[i];
		robot->TestSelection(num);

		robot->Intermission();

	}
	robot->SetState(3);
	return 0;
}

DWORD WINAPI ThreadManager(LPVOID lpParam)
{
	Operator* op = static_cast<Operator*>(lpParam);
	HANDLE hThread = GetCurrentThread;


	while (true)
	{
		std::cout << "p - pause\nr - resume\nq - quit ";
		char c;
		std::cin >> c;

		std::cout << "Введите id робота: ";
		int id;
		std::cin >> id;

		if (!(id < 0 || id > op->robotAmount))
		{
			switch (c)
			{
			case 'p':
				SuspendThread(op->threads[id]);
				std::cout << "Робот номер " << id << " приостановлен\n";
				op->robots[id].SetState(2);
				break;
			case 'r':
				ResumeThread(op->threads[id]);
				std::cout << "Робот номер " << id << " возобновил свою работу\n";
				op->robots[id].SetState(1);
				break;
			case 'q':
				ExitThread(0);
				CloseHandle(hThread);
				break;
			default:
				break;
			}
		}
		else
		{
			std::cout << "Некорректный id\n";
			continue;
		}



	}

	return 0;
}



int main()
{
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));
	
	int robotCount = 3 + rand() % 8;

	

	random_device rd;

	Operator op;



	std::ifstream file("arena.dat", std::ios::binary);

	if (file.is_open())
	{
		file.seekg(0, ios::end);

		int testCount = file.tellg() / sizeof(int);

		file.seekg(0, ios::beg);

		int* arenaNums = new int[testCount];

		file.read(reinterpret_cast<char*>(arenaNums), testCount * sizeof(int));

		for (int i = 0; i < testCount; i++)
		{
			cout << arenaNums[i] << "\n";
		}

		file.close();





		Robot* robots = new Robot[robotCount];

		HANDLE* threads = new HANDLE[robotCount];

		for (int i = 0; i < robotCount; i++)
		{

			robots[i].SetId(i);
			robots[i].SetState(1);
			robots[i].testNums = arenaNums;
			robots[i].mt.seed(rd());
			
			
		}



		for (int i = 0; i < robotCount; i++)
		{

			threads[i] = CreateThread(NULL, 0, StartTest, &robots[i], 0, NULL); // почему нет проверки на создание потока? 
			if (threads[i] == NULL)
			{
				ExitProcess(3);
			}
			Sleep(50);

		}

		op.robotAmount = robotCount;
		op.threads = threads;
		op.robots = robots;

		HANDLE hOp = CreateThread(NULL, 0, ThreadManager, &op, 0, NULL); // почему нет проверки на создание потока? 

		if (hOp == NULL)
		{
			ExitProcess(3);
		}

		WaitForMultipleObjects(robotCount, threads, TRUE, INFINITE);

		for (int i = 0; i < robotCount; i++)
		{
			CloseHandle(threads[i]);
		}

		for (int i = 0; i < robotCount - 1; i++)
		{
			for (int j = 0; j < robotCount - 1 - i; j++)
			{
				if (robots[j].GetScore() < robots[j + 1].GetScore())
				{
					Robot temp = robots[j];
					robots[j] = robots[j + 1];
					robots[j + 1] = temp;
				}
			}
		}

		std::cout << "\nТаблица роботов:\n";

		int maxScore = 0;
		int countMaxScore = 0;

		for (int i = 0; i < robotCount; i++)
		{
			if (robots[i].GetScore() > maxScore)
			{
				maxScore = robots[i].GetScore();
			}

		}

		
		for (int i = 0; i < robotCount; i++)
		{
			if (robots[i].GetScore() == maxScore)
			{
				countMaxScore++;
				
			}

			std::cout << "Робот id: " << robots[i].GetId() <<
				"\nОчки: " << robots[i].GetScore() <<
				"\nЭнергия: " << robots[i].GetEnergy() <<
				"\nСостояние: " << robots[i].GetState() << std::endl;
		}

		if (countMaxScore == 1)
		{
			std::cout << "\n\nРобот-победитель\n id: " << robots[0].GetId() <<
				"\nОчки: " << robots[0].GetScore() <<
				"\nЭнергия: " << robots[0].GetEnergy() <<
				"\nСостояние: " << robots[0].GetState() << std::endl;
		}
		else
		{
			std::cout << "Ничья" << std::endl;
		}


		CloseHandle(hOp);


		delete[] robots;
		delete[] threads;
		delete[] arenaNums;




	}

}



// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
