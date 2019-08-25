#include "pch.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
using namespace std;


const char *pipename = "\\\\.\\pipe\\pipe";
const char *path = "C:\\DMS";

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

char *createMessageForPipe(int num)
{
	char *message;
	if (num == 1)
	{
		cout << 'c';
		int len = strlen(DocName) + strlen(Author) + strlen(id) + 3;
		message = new char[len];
		*message = 0;
		strcat(message, DocName);
		strcat(message, ";");
		strcat(message, Author);
		strcat(message, ";");
		strcat(message, id);
		strcat(message, ";");
		return message;
	}
	//cout << 'c';
	char err[4] = "000";
	return err;
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
	//cout << endl << strlen(str1);
	return str1;
}

char **parseMessage(char message[], int num)
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

DWORD  cbRead, cbWritten, dwMode;


int newDoc(HANDLE pipe) {
	char path[100] = { 0 };
	bool check = false;
	cout << "=== Создание нового файла ===" << endl;
	
	cout << "Путь к файлу" << endl;

	while (!check) {
		cout << "> ";
		cin >> path;

		WIN32_FIND_DATA fData = {};
		HANDLE dir = FindFirstFile(path, &fData);
		
		if (dir != INVALID_HANDLE_VALUE) {
			WriteFile(pipe, &path, sizeof(path), &cbWritten, NULL);
			cout << endl << path << endl;
			check = true;
		}
		else 
			cout << endl << "Такого файла не существует, попробуйте еще раз" << endl;
	}

	cout << "Имя файла" << endl;
	cout << "> ";
	cin >> DocName;

	cout << "Автор файла" << endl;
	cout << "> ";
	cin >> Author;

	cout << "id файла" << endl;
	cout << "> ";
	cin >> id;
	
	//Не знаю почему,но без чтения данных, программа не правильно начинает работать и в метаданные ничего не записывается
	//char *message0 = parseMessage(mes, 1)[0];
	//char *message1 = parseMessage(mes, 1)[1];
	//char *message2 = parseMessage(mes, 1)[2];
	//cout << endl << message0;
	//cout << endl << message1;
	//cout << endl << message2;

	char mes1[400] = { 0 };
	char mes2[4] = "asd";

	strcat(mes1, createMessageForPipe(1));

	WriteFile(pipe, &mes1, sizeof(mes1), &cbWritten, NULL);
	cout << endl << mes1 << endl;
	//cout << endl << newmd.DocName << ' ' << newmd.Author << ' ' << newmd.id << ' ' << endl;
	return 0;
}

int cngDoc(HANDLE pipe) {
	char path[100];
	char name[100];
	bool exist = false;
	bool check = false;
	cout << "=== Изменение файла ===" << endl;
	cout << "Имя файла" << endl;
	while (!exist) {
		cout << "> ";
		cin >> name;

		WriteFile(pipe, &name, sizeof(name), &cbWritten, NULL);
		ReadFile(pipe, &exist, 1, &cbRead, NULL);

		if (!exist) {
			char user = '3';
			cout << "Данного документа не существует, введите другой документ" << endl;
			WriteFile(pipe, &user, sizeof(user), &cbWritten, NULL);
		}
	}

	cout << "Путь к файлу, который изменит документ введенный ранее" << endl;

	while (!check) {
		cout << "> ";
		cin >> path;

		WIN32_FIND_DATA fData = {};
		HANDLE dir = FindFirstFile(path, &fData);

		if (dir != INVALID_HANDLE_VALUE) {
			WriteFile(pipe, &path, sizeof(path), &cbWritten, NULL);
			check = true;
		}
		else
			cout << endl << "Такого файла не существует, попробуйте еще раз" << endl;
	}


	return 0;
}

int cngMetaData(HANDLE pipe) {
	char name[100];
	bool exist = false;

	cout << "=== Изменение метаданных файла ===" << endl;

	cout << "Введите наименование документа, метаданные которого вы хотите изменить" << endl;
	while (!exist) {
		cout << "> ";
		cin >> name;

		WriteFile(pipe, &name, sizeof(name), &cbWritten, NULL);
		ReadFile(pipe, &exist, 1, &cbRead, NULL);

		if (!exist) {
			char user = '4';
			cout << "Данного документа не существует, введите другой документ" << endl;
			WriteFile(pipe, &user, sizeof(user), &cbWritten, NULL);
		}
	}

	cout << "Имя файла" << endl;
	cout << "> ";
	cin >> DocName;

	cout << "Автор файла" << endl;
	cout << "> ";
	cin >> Author;

	cout << "id файла" << endl;
	cout << "> ";
	cin >> id;

	char mes[400] = { 0 };

	strcat(mes, createMessageForPipe(1));
	WriteFile(pipe, &mes, sizeof(mes), &cbWritten, NULL);

	return 0;
}

int opnDoc(HANDLE pipe) {
	char name[100];//название документа
	int read;//сколько читать
	bool exist = false;//Существует ли файл

	cout << "=== Открыть файл ===" << endl;

	cout << "Имя файла" << endl;
	while (!exist) {
		cout << "> ";
		cin >> name;

		WriteFile(pipe, &name, sizeof(name), &cbWritten, NULL);
		ReadFile(pipe, &exist, 1, &cbRead, NULL);

		if (!exist) {
			char user = '2';
			cout << "Данного документа не существует, попробуйте ввести другой документ" << endl;
			WriteFile(pipe, &user, sizeof(user), &cbWritten, NULL);
		}
	}
	ReadFile(pipe, &read, sizeof(read), &cbRead, NULL);
	char content[100];//то что было внутри файла
	ReadFile(pipe, &content, 100, &cbRead, NULL);
	cout << content << endl;


	return 0;
}

int List(HANDLE pipe) {
	char doc[100] = { 0 };

	cout << "=== Все файлы ===" << endl;

	do{
		*doc = NULL;
		ReadFile(pipe, &doc, 100, &cbRead, NULL);

		cout << doc << endl;
	} while (strcmp(doc, "End of file") != 0);

	return 0;
}


int Menu() {
	char user;
	cout << "1 - Добавить новый документ" << endl;
	cout << "2 - Открыть файл" << endl;
	cout << "3 - Изменить тело документа" << endl;
	cout << "4 - Изменить метаданные документа" << endl;
	cout << "5 - Просмотреть все файлы" << endl;
	cout << "0 - Выход" << endl;
	cout << "> ";
	cin >> user;
	return user;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	

	//Метаданные
	//===============
	char DocName[200] = { 0 }; // Название документа
	char Author[100] = { 0 }; // Автор документа
	char id[100] = { 0 }; // id документа
	//===============

	HANDLE pipe;
	char user;

	while (true) {

		pipe = CreateFileA(pipename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);


		if (pipe == INVALID_HANDLE_VALUE) {
			cout << "Произошел сбой в подключеннии к серверу." << endl;
			cout << "ERROR = " << GetLastError();
			return -1;
		}

		user = Menu();
		if (user == '0')
			return 0;


		cout << endl << user << endl;

		WriteFile(pipe, &user, 1, &cbWritten, NULL);

		switch (user) {
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
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
