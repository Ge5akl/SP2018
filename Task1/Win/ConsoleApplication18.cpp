// ConsoleApplication18.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <windows.h>
#include <stdio.h>


using namespace std;

int main(int argc, char* argv[])
{
	char *arg[] = { NULL };
	int in = 1;
	char *maxIn = getenv("MAX_INSTANCES");
	bool w = false;

	for (int i = 0; i < argc; i++) {
		if ((string(argv[i]) == "-w") || (string(argv[i]) == "--wait")) w = true; 
		else if ((string(argv[i]) == "-i") || (string(argv[i]) == "--instances")) {
			in = atoi(argv[++i]);
			(maxIn != NULL) ? ((atoi(maxIn) < in) ? in = atoi(maxIn) : false) : false;
		}
		else if ((string(argv[i]) == "-a") || (string(argv[i]) == "--arguments")) {
			*arg = argv[++i];
		}
	}

	HANDLE *handles = new HANDLE[in]; 

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	for (int i = 0; i < in; i++) {
		 if (argc > 1) { 
			CreateProcess(argv[argc - 1], arg[0], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
			handles[i] = pi.hProcess; 
		}
	 }

	if (w) WaitForMultipleObjects(in, handles, TRUE, INFINITE);

	cout << "complete";

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	delete[] handles;

	return 0;
}
