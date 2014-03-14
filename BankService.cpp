// BankService.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "windows.h"
#include <fstream>
#include <queue>
#include <vector>
#include "BankService.h"
#define CUSNUM 8
#define SERVNUM 3
#define INTERVAL 1

using namespace std;



int service[SERVNUM];
vector<customer> customers;
queue<customer *> cusQueue;
queue<int> servQueue;
HANDLE Mutex = CreateSemaphore(NULL,1,1,NULL);
HANDLE Simul = CreateSemaphore(NULL,SERVNUM,SERVNUM,NULL);
HANDLE thread[SERVNUM+2];
DWORD threadID[SERVNUM+2];
int currentTime;

void servFunc(customer * cus)
{
	Sleep(cus->serveTime * INTERVAL);
	if (WaitForSingleObject(Mutex,INFINITE) == WAIT_OBJECT_0)
	{
		servQueue.push(cus->servNum);
		ReleaseSemaphore(Simul,1,NULL);
		FILE * fpw;	
		fpw = fopen("output.txt","a+");
		fprintf(fpw,"%d ",cus->enterTime);
		fprintf(fpw,"%d ",cus->startTime);
		fprintf(fpw,"%d ",cus->serveTime+cus->startTime);
		fprintf(fpw,"%d \n",cus->servNum);

		fclose(fpw);
		ReleaseSemaphore(Mutex,1,NULL);
	}

}

void customerFunc()
{
	while(1)
	{
		if(cusQueue.size()==0)
			continue;
		if (WaitForSingleObject(Simul,INFINITE) == WAIT_OBJECT_0)
		{
			cusQueue.front()->servNum = servQueue.front();
			cusQueue.front()->startTime = currentTime;
			printf("customer %d gets service from time %d to time %d\n",cusQueue.front()->seqNum,cusQueue.front()->startTime
				,cusQueue.front()->startTime+cusQueue.front()->serveTime);
			thread[servQueue.front()+1] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)(servFunc),cusQueue.front(),0,&threadID[servQueue.front()+1]);
			cusQueue.pop();
			servQueue.pop();
			ReleaseSemaphore(Mutex,1,NULL);
		}
	}


}
void customerIn()
{
	int currentNum = 1;
	while(1)
	{
		if(currentNum > customers.size())
			continue;
		if(customers[currentNum -1].enterTime == currentTime)
		{
			if (WaitForSingleObject(Mutex,INFINITE) == WAIT_OBJECT_0)  // succeed
			{
				cusQueue.push(&customers[currentNum-1]);
				printf("customer %d arrives at time %d \n",currentNum,currentTime);
				currentNum++;
				ReleaseSemaphore(Mutex,1,NULL);

			}
		}
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	
	/*FILE * fpw;	
		fpw = fopen("input.txt","w");
		for(int i = 1; i < 50; i++)
		{
		fprintf(fpw,"%d ",i);
		fprintf(fpw,"%d ",rand()%100);
		fprintf(fpw,"%d \n",rand()%10);
		}
		fclose(fpw);*/
	
	
	FILE * fp;
	//customer cus[CUSNUM];
	fp = fopen("input.txt","r");
	if (fp == NULL)
		printf("cannot open");
	for(int i = 0; i < CUSNUM; i++)
	{
		
		customer temp;
		//temp = new customer;
		fscanf(fp,"%d",&temp.seqNum);
		fscanf(fp,"%d",&temp.enterTime);
		fscanf(fp,"%d",&temp.serveTime);
		customers.push_back(temp);
		
	}
	fclose(fp);
	for (int i = 0; i < SERVNUM; i++)
		servQueue.push(i);
	printf("there are %d services\n",SERVNUM);


	thread[0] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)(customerIn),NULL,0,&threadID[0]);
	thread[1] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)(customerFunc),NULL,0,&threadID[1]);
	currentTime = 0;

	while(1)
	{
		Sleep(INTERVAL);
		currentTime++;
	}
	
	return 0;
}

