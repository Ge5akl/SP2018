#include "pch.h"
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <process.h>
#include <string>
#include <fstream>
using namespace std;

const char *filepath = "C:\\DMS\\";
const char *docpath = "C:\\DMS\\Docs\\";
const char *metapath = "C:\\DMS\\MetaData\\";

const char *pipename = "\\\\.\\pipe\\pipe";

DWORD  cbWritten = 0;
DWORD  cbRead = 0;

struct MetaData
{
	char DocName[200] = { 0 }; // Название документа
	char Author[100] = { 0 }; // Автор документа
	char id[100] = { 0 }; // id документа
};

MetaData md;

//Метаданные
//==================================================
char DocName[200] = { 0 }; // Название документа
char Author[100] = { 0 }; // Автор документа
char id[100] = { 0 }; // id документа
//==================================================

char *createMessageForPipe(int num) {
	char *message = new char[strlen(DocName) + strlen(Author) + strlen(id) + 3];
	strcat(message, DocName);
	strcat(message, ";");
	strcat(message, Author);
	strcat(message, ";");
	strcat(message, id);

	return message;
}

char *strn(char string[], int start, int end)
{
	char *str1 = new char[end - start];

	int i = 0;
	while (start != end)
	{
		str1[i] = string[start++];
		//cout << str[i];
		i++;
	}
	str1[i] = '\0';
	cout << endl << strlen(str1);
	return str1;
}

char **parseMessage(char message[])
{
	int i = 0, j = 0, start = 0, end;
	int len = strlen(message);
	char **str = new char*[len - 1];
	while (i < len)
	{
		if (message[i] == ';')
		{
			end = i;
			str[j] = new char[end - start];
			//cout << endl << end << ' ' << start;
			str[j] = strn(message, start, end);

			//cout << endl << str[j] << ' ' << end << ' ' << start;
			start = i + 1;

			j++;
		}
		i++;
	}

	return str;
}

int newDoc(HANDLE pipe) {
	char path[100] = { 0 };

	ReadFile(pipe, &path, sizeof(path), &cbRead, NULL);

	ifstream in;
	ofstream out;

	//чтение файла

	in.open(path, ios_base::binary | ios_base::ate);
	if (in.is_open() != 1) cout << "Error: file isn't open";
	int size = in.tellg();
	in.close();

	char *content = new char[sizeof(char) * size];

	in.open(path, ios_base::binary | ios_base::in);
	in.read(content, size);
	in.close();
	int size1 = 0;
	ReadFile(pipe, &size1, sizeof(size1), &cbRead, NULL);
	char *mes = new char[size1 + 1];
	//*mes = { 0 };
	char mes1[400] = { 0 };

	ReadFile(pipe, &mes1, sizeof(mes1), &cbRead, NULL);
	size1 = 1;
	WriteFile(pipe, &mes1, sizeof(mes1), &cbWritten, NULL);
	//WriteFile(pipe, &md, sizeof(md), &cbWritten, NULL);

	//cout << md.DocName << ' ' << md.Author << ' ' << md.id << ' ' << endl;

	//Запись в документ


	char *message0 = parseMessage(mes1)[0];
	char *message1 = parseMessage(mes1)[1];
	char *message2 = parseMessage(mes1)[2];

	char *newFile = new char[strlen(docpath) + strlen(message0) + strlen(".txt") + 1];

	*newFile = { 0 };

	//*DocName = *message0;
	//*Author = *message1;
	//*id = *message2;

	strcat(newFile, docpath);
	strcat(newFile, message0);
	strcat(newFile, ".txt");

	cout << newFile << ' ' << content << endl;

	out.open(newFile);
	out.write(content, size);
	out.close();

	newFile = new char[strlen(metapath) + strlen(message0) + strlen("_metadata.txt") + 1];

	*newFile = { 0 };
	strcat(newFile, metapath);
	strcat(newFile, message0);
	strcat(newFile, "_metadata.txt");

	out.open(newFile);
	out << "Doc Name = " << message0 << '\n';
	out << "Author = " << message1 << '\n';
	out << "id of document = " << message2 << '\n';
	out.close();

	return 0;
}

int cngDoc(HANDLE pipe) {
	char path[100] = { 0 };
	char name[100];
	char exist = '0';
	bool check = false;
	bool check1 = true;

	ReadFile(pipe, &name, sizeof(name), &cbRead, NULL);
	strcat(name, ".txt");

	strcat(path, docpath);
	strcat(path, "*.*");
	WIN32_FIND_DATA fData = {};
	HANDLE dir = FindFirstFileA(path, &fData);
	cout << path << endl;

	while (FindNextFile(dir, &fData) != false) {
		char *FileName = fData.cFileName;

		cout << name << ' ' << FileName << ' ' << strcmp(FileName, name) << endl;

		exist = false;
		if (strcmp(FileName, name) == 0) {
			exist = true;
			check = true;
			WriteFile(pipe, &exist, sizeof(exist), &cbWritten, NULL);
			break;
		}
	}
	if (!check) {
		*name = NULL;
		WriteFile(pipe, &exist, sizeof(exist), &cbWritten, NULL);
		return -1;
	}
	FindClose(dir);
	*path = { 0 };

	ReadFile(pipe, &path, sizeof(path), &cbRead, NULL);

	//Чтение файла

	ifstream in;
	ofstream out;

	in.open(path, ios_base::binary | ios_base::ate);
	if (in.is_open() != 1) cout << "Error: file isn't open";
	int size = in.tellg();
	in.close();

	char *content = new char[size];

	//*content = {0};

	in.open(path, ios_base::binary | ios_base::in);
	in.read(content, size);
	in.close();

	cout << size << endl;

	//Изменение документа

	char *newFile = new char[strlen(docpath) + strlen(name) + 1];

	*newFile = { 0 };

	strcat(newFile, docpath);
	strcat(newFile, name);

	cout << newFile << ' ' << content << endl;

	out.open(newFile);
	out.write(content, size);
	out.close();

	return 0;
}

int cngMetaData(HANDLE pipe) {
	char path[100] = { 0 };
	char name[100];
	char exist = '0';
	bool check = false;
	bool check1 = true;

	ReadFile(pipe, &name, sizeof(name), &cbRead, NULL);
	strcat(name, "_metadata.txt");

	strcat(path, metapath);
	strcat(path, "*.*");
	WIN32_FIND_DATA fData = {};
	HANDLE dir = FindFirstFileA(path, &fData);
	cout << path << endl;

	while (FindNextFile(dir, &fData) != false) {
		char *FileName = fData.cFileName;

		cout << name << ' ' << FileName << ' ' << strcmp(FileName, name) << endl;

		exist = false;
		if (strcmp(FileName, name) == 0) {
			exist = true;
			check = true;
			WriteFile(pipe, &exist, sizeof(exist), &cbWritten, NULL);
			break;
		}
	}
	if (!check) {
		*name = NULL;
		WriteFile(pipe, &exist, sizeof(exist), &cbWritten, NULL);
		return -1;
	}
	FindClose(dir);
	*path = { 0 };

	ofstream out;

	//Изменение документа

	char *newFile = new char[strlen(metapath) + strlen(name) + 1];

	*newFile = { 0 };

	strcat(newFile, metapath);
	strcat(newFile, name);

	cout << newFile << endl;

	int size1 = 0;
	ReadFile(pipe, &size1, sizeof(size1), &cbRead, NULL);
	char *mes = new char[size1];
	ReadFile(pipe, &mes, size1, &cbRead, NULL);

	out.open(newFile);
	out << "Doc Name = " << message0 << '\n';
	out << "Author = " << message1 << '\n';
	out << "id of document = " << message2 << '\n';
	out.close();

	return 0;
}

int opnDoc(HANDLE pipe) {
	setlocale(LC_ALL, "Russian");


	char name[105];//наименование искомого файла
	char path[200] = { 0 };//Путь к файлу
	bool exist = false;

	ReadFile(pipe, &name, sizeof(name), &cbRead, NULL);
	strcat(name, ".txt");

	strcat(path, docpath);
	strcat(path, name);

	WIN32_FIND_DATA fData = {};
	HANDLE dir = FindFirstFile(path, &fData);

	if (strcmp(fData.cFileName, name) != 0) {
		WriteFile(pipe, &exist, sizeof(exist), &cbWritten, NULL);

		return -1;
	}
	exist = true;
	WriteFile(pipe, &exist, sizeof(exist), &cbWritten, NULL);

	ifstream in;//Для чтения

	//чтение файла
	in.open(path, ios_base::binary | ios_base::ate);
	if (in.is_open() != 1) cout << "Error: file isn't open";
	int size = in.tellg();
	in.close();

	char content[100] = { 0 };

	in.open(path, ios_base::binary | ios_base::in);
	in.read(content, size);
	in.close();

	cout << content << endl;
	WriteFile(pipe, &size, sizeof(size), &cbWritten, NULL);
	WriteFile(pipe, &content, 100, &cbWritten, NULL);

	return 0;
}

int List(HANDLE pipe) {
	bool endlist = true;
	char path[200] = { 0 };

	strcat(path, docpath);
	strcat(path, "*.*");

	WIN32_FIND_DATA fData = {};
	HANDLE dir = FindFirstFile(path, &fData);
	char *FileName = fData.cFileName;

	cout << FileName << endl;
	//WriteFile(pipe, &FileName, 100, &cbWritten, NULL);

	while (FindNextFile(dir, &fData) != false) {
		FileName = NULL;
		FileName = fData.cFileName;
		cout << FileName << endl;
		if (FileName != "." || FileName != "..")
			WriteFile(pipe, &fData.cFileName, 100, &cbWritten, NULL);
	}
	cout << "end" << endl;
	*path = { 0 };
	strcpy(path, "End of file");

	WriteFile(pipe, &path, sizeof(path), &cbWritten, NULL);
	FindClose(dir);

	return 0;
}

DWORD WINAPI InstanceThread(LPVOID lpvParam)
{
	BOOL fSuccess = FALSE;
	HANDLE pipe = NULL;
	//WIN32_FIND_DATA fdat;
	//HANDLE hdata;

	*Author = { 0 };
	*DocName = { 0 };
	*id = { 0 };

	CreateDirectoryA(docpath, NULL); //Создание папок
	CreateDirectoryA(metapath, NULL); // где будут храняться данные
	setlocale(LC_ALL, "Russian");

	char user;
	int usr = 1;

	if (lpvParam == NULL)
	{
		cout << "ERROR pipe is NULL";
		return -1;
	}

	pipe = (HANDLE)lpvParam;

	while (true)
	{
		ReadFile(pipe, &user, 1, &cbRead, NULL);

		if (user == '0')
			break;

		switch (user)
		{
			case '1':
				newDoc(pipe);
				break;
			case '2':
				opnDoc(pipe);
				break;
			case '3':
				cngDoc(pipe);
				break;
			case '4':
				cngMetaData(pipe);
				break;
			case '5':
				List(pipe);
				break;
		}
	}

	FlushFileBuffers(pipe);
	DisconnectNamedPipe(pipe);
	CloseHandle(pipe);

	return 1;
}

//===============================================================================================

DWORD WINAPI HandlerEx(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext);

static SERVICE_STATUS ssServiceStatus;
static bool bServiceStopFlag = false;
static SERVICE_STATUS_HANDLE hService;

void WINAPI ServiceMain(DWORD argc, LPTSTR argv[])
{
	hService = RegisterServiceCtrlHandlerEx(ServiceName, HandlerEx, NULL);

	ssServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	ssServiceStatus.dwCurrentState = SERVICE_RUNNING;
	ssServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	ssServiceStatus.dwWin32ExitCode = 0;
	ssServiceStatus.dwServiceSpecificExitCode = 0;
	ssServiceStatus.dwCheckPoint = 0;
	ssServiceStatus.dwWaitHint = 0;

	SetServiceStatus(hService, &ssServiceStatus);

	*md.DocName = { 0 };
	*md.Author = { 0 };
	*md.id = { 0 };

	DWORD  dwThreadId = 0;

	while (true) {
		HANDLE pipe =
			CreateNamedPipeA(
				pipename,
				PIPE_ACCESS_DUPLEX,
				PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
				PIPE_UNLIMITED_INSTANCES,
				512,
				512,
				5000,
				NULL);
		if (pipe == INVALID_HANDLE_VALUE)
		{
			cout << "Error = " << GetLastError();
			return;
		}

		if (ConnectNamedPipe(pipe, NULL))
		{
			HANDLE thread;
			thread = CreateThread(
				NULL,              // no security attribute
				0,                 // default stack size
				InstanceThread,    // thread proc
				(LPVOID)pipe,    // thread parameter
				0,                 // not suspended
				&dwThreadId);      // returns thread ID

			if (thread == NULL)
			{
				cout << "Error = " << GetLastError();
				return;
			}
			else CloseHandle(thread);
		}
		else
			CloseHandle(pipe);
	}
	//Sleep(1000);
	ssServiceStatus.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(hService, &ssServiceStatus);
}

DWORD WINAPI HandlerEx(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext)
{
	switch (dwControl)
	{
	case SERVICE_CONTROL_SHUTDOWN:
	case SERVICE_CONTROL_STOP:
		ssServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
		ssServiceStatus.dwCheckPoint = 0;
		ssServiceStatus.dwWaitHint = 80000;

		SetServiceStatus(hService, &ssServiceStatus);
		bServiceStopFlag = true;

		break;

	case SERVICE_CONTROL_INTERROGATE:
		SetServiceStatus(hService, &ssServiceStatus);
		break;
	}

	return 1;
}