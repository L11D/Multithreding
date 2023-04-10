#pragma once
#include <csignal>
#include <thread>
#include <queue>
#include <windows.h>
#include <mutex>
#include "SupportFunctions.h"

using namespace std;

void deviceLogic(volatile sig_atomic_t& endSignal, priority_queue<int>& queue, mutex& queueMutex, int& counter, mutex& counterM)
{
	while (endSignal == 0)
	{
		queueMutex.lock();
		if (!queue.empty())
		{
			queue.pop();
			queueMutex.unlock();
			Sleep(getRandom(10, 1000));
			counterM.lock();
			if (counter > 0)
			{
				counter--;
				counterM.unlock();
			}
			else
			{
				counterM.unlock();
			}
		}
		else
		{
			queueMutex.unlock();
		}
	}
}

class Device
{
public:

	Device(priority_queue<int>& queue, volatile sig_atomic_t& endSignal, mutex& queueMutex, int& counter, mutex& counterM)
		: endSignal(endSignal)
	{
		deviceTread = thread(deviceLogic, ref(endSignal), ref(queue), ref(queueMutex), ref(counter), ref(counterM));
	}

	void wait()
	{
		deviceTread.join();
	}


private:
	volatile sig_atomic_t& endSignal;
	thread deviceTread;
};

