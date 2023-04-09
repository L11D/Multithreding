#pragma once
#include <csignal>
#include <thread>
#include <queue>
using namespace std;
class Device
{
private:
	int groupIndex;
	priority_queue<int>& queue;
public:
	Device(priority_queue<int>& deviceQueue, int a)
	{
		queue = deviceQueue;
	}
	//~Device();


};

