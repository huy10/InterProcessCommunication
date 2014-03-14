#include "stdafx.h"
#include "windows.h"
#include <fstream>
#include <queue>
#include <vector>
#define CUSNUM 3
#define SERVNUM 2
#define INTERVAL 1000

class customer
{
public:
	int seqNum;
	int servNum;
	int enterTime;
	int startTime;
	int serveTime;
	customer(){}
	~customer(){}
};
