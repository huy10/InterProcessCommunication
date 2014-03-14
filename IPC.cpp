// IPC.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <fstream>
#include <iostream>
#include <process.h>
#define N 8
//#define long message
using namespace std;


//int itr_m = 0, itr_n = 0;
//MSG _message[2] = {NULL,NULL};
HANDLE thread[2] = {NULL,NULL};
HANDLE read[2] = {NULL,NULL};
HANDLE write[2] = {NULL,NULL};
DWORD threadID[2] = {0,0};

DWORD WINAPI ffun(PVOID lpParam)
{
	ULONG64 m = *(PULONG64) lpParam;
	ULONG64 _m = m;
	ULONG64 fm = 1;
	while (_m > 1)
		fm *= (_m--);
	//PeekMessage(&_message[0], NULL, 0,0, PM_NOREMOVE);
	//BOOL p0  = PostThreadMessage(threadID[0],(UINT)fm,0,0);  //(HWND)threadID[0]
	//Sleep(1000);
	DWORD NumberOfBytesWritten = 0;
	WriteFile(write[0],&fm,N,&NumberOfBytesWritten,NULL);
	//cout << GetLastError();
	return 0;

}

DWORD WINAPI gfun(PVOID lpParam)
{
	ULONG64 n = *(PULONG64) lpParam;
	ULONG64 gn[2] = {1,1};
     // here implicitly n > 2
	int i = 0;
	for ( i = 0;i < n - 2; i++)
		gn[i%2] = gn[0] + gn[1];
	//PeekMessage(&_message[1], NULL, 0,0, PM_NOREMOVE);
	//BOOL p1 = PostThreadMessage(threadID[1],(UINT)gn[1-n%2],0,0);
	DWORD NumberOfBytesWritten = 0;
	WriteFile(write[1],&gn[1-n%2],N,&NumberOfBytesWritten,NULL);
	//cout << GetLastError();
	//Sleep(1000);
	return 0;
	
}

int _tmain(int argc, _TCHAR* argv[])
{

	ULONG64 m = 6, n = 10 ;
	
	CreatePipe(&read[0],&write[0],NULL,N);
	CreatePipe(&read[1],&write[1],NULL,N);


	thread[0] = CreateThread(NULL,0,ffun,&m,0,&threadID[0]);
	thread[1] = CreateThread(NULL,0,gfun,&n,0,&threadID[1]);
	
	
	//cout << threadID[0] << " " << threadID[1] << endl;
	ULONG64 fm = 0, gn = 0;
	DWORD NumberOfBytesRead = 0;
	ReadFile(read[0],&fm,N,&NumberOfBytesRead,NULL);
	ReadFile(read[1],&gn,N,&NumberOfBytesRead,NULL);

	//MSG _fm,_gn;
	//BOOL b0 =  GetMessage(&_fm,(HWND)threadID[0],0,0);   //(HWND)thread[0]
	// cout << GetLastError();
	//BOOL b1 =   GetMessage(&_gn,(HWND)threadID[1],0,0);
	// cout << GetLastError();

	//fm = _message[0];
	//gn = _message[1];
	 WaitForMultipleObjects(2,thread,TRUE,INFINITE);
	cout << "Result: " << fm + gn<<endl;
	
	return 0;
}

