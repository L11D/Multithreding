#pragma once
#include<iostream>

using namespace std;

int getRandom(int min, int max)
{
	return min + rand() % (max - min + 1);
}
