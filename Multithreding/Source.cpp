#include<iostream>
#include<queue>
#include<vector>
#include<thread>
#include<mutex>
#include <csignal>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include "Device.h"
#include "SupportFunctions.h"
using namespace std;

volatile sig_atomic_t endSignal;
int groupCount = 10;
int deviceCount = 3;
int maxQueueSize = 300;
int currentQueueSize = 0;
mutex currentQueueSizeM;

void signalHendler(int signal)
{
	endSignal = signal;
}

void printQueue(vector<priority_queue<int>> queueGroups)
{
	cout << "Queue:" << endl;
	for (int i = 0; i < queueGroups.size(); i++)
	{
		cout << "\tGroup " << i << ": ";
		while (!queueGroups[i].empty())
		{
			cout << queueGroups[i].top() << " ";
			queueGroups[i].pop();
		}
		cout << endl;
	}
}

void generatorLogic(vector<priority_queue<int>>& queueGroups, vector<mutex>& queueMutexes, mutex& currentQueueSizeM)
{
	while (endSignal == 0)
	{
		currentQueueSizeM.lock();
		if (currentQueueSize < maxQueueSize)
		{
			currentQueueSize++;
			currentQueueSizeM.unlock();
			int groupIndex = getRandom(0, queueGroups.size() - 1);
			int priority = getRandom(0, 2);
			queueMutexes[groupIndex].lock();
			queueGroups[groupIndex].push(priority);
			queueMutexes[groupIndex].unlock();
		}
		else
		{
			currentQueueSizeM.unlock();
		}
	}
}

int main()
{
	cout << "Enter groups count: "; cin >> groupCount;
	cout << "Enter devices count: "; cin >> deviceCount; 
	cout << "Enter queue size: "; cin >> maxQueueSize; 

	srand(time(NULL));
	signal(SIGINT, signalHendler);

	vector<priority_queue<int>> queueGroups(groupCount);
	vector<mutex> queueMutexes(groupCount);

	thread generator(generatorLogic, ref(queueGroups), ref(queueMutexes), ref(currentQueueSizeM));

	vector<vector<Device>> devicesGroups(groupCount);
	for (int i = 0; i < devicesGroups.size(); i++)
	{
		for (int j = 0; j < deviceCount; j++)
		{
			devicesGroups[i].push_back(Device(queueGroups[i], endSignal, queueMutexes[i], currentQueueSize, currentQueueSizeM));
		}
	}

	while (endSignal == 0)
	{
		printQueue(queueGroups);
		Sleep(30);
		system("cls");
	}

	generator.join();
	for (int i = 0; i < devicesGroups.size(); i++)
	{
		for (int j = 0; j < devicesGroups[i].size(); j++)
		{
			devicesGroups[i][j].wait();
		}
	}

	printQueue(queueGroups);
}