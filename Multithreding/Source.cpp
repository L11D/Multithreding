#include<iostream>
#include<queue>
#include<vector>
#include<thread>
#include <csignal>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include "Device.h"
using namespace std;

volatile sig_atomic_t endSignal;
int groupCount = 10;
int deviceCount = 3;
int maxQueueSize = 100;
int currentQueueSize = 0;

void signalHendler(int signal)
{
	endSignal = signal;
}

int getRandom(int min, int max)
{
	return min + rand() % (max - min + 1);
}

void generateRequest(vector<priority_queue<int>>& queueGroups)
{
	while (endSignal == 0)
	{
		if (currentQueueSize < maxQueueSize)
		{
			currentQueueSize++;
			int groupIndex = getRandom(0, queueGroups.size() - 1);
			int priority = getRandom(0, 10);
			queueGroups[groupIndex].push(priority);
		}
	}
}

void deviceLogic(vector<priority_queue<int>>& queueGroups, int groupIndex)
{
	if(!queueGroups[groupIndex].empty())
		queueGroups[groupIndex].pop();
	Sleep(1000);
	cout << "end" << endl;
}

void printQueue(vector<priority_queue<int>> queueGroups)
{
	for (int i = 0; i < queueGroups.size(); i++)
	{
		cout << "Group " << i << ": ";
		while (!queueGroups[i].empty())
		{
			cout << queueGroups[i].top() << " ";
			queueGroups[i].pop();
		}
		cout << endl;
	}
}

int main()
{
	srand(time(NULL));
	signal(SIGINT, signalHendler);

	vector<priority_queue<int>> queueGroups(groupCount);
	//thread t1(generateRequest, ref(queueGroups));
	thread t2(deviceLogic, ref(queueGroups), 0);
	t2.join();
	//generateRequest(queueGroups);
	Sleep(100);

	printQueue(queueGroups);

	//priority_queue<Request> queue;
	//t1.join();

}