#include "pch.h"
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <process.h>
#include <string>
#include <fstream>
using namespace std;

void WINAPI ServiceMain(DWORD argc, LPTSTR argv[]);

int _tmain()
{
	SERVICE_TABLE_ENTRY ServiceTable[] =
	{
		{ ServiceName, ServiceMain },
		{ NULL, NULL }
	};

	if (!StartServiceCtrlDispatcher(ServiceTable))
	{
		printf("Service not Started!");
		
		return -1;
	}

	return 0;
}