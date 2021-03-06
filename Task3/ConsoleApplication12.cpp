#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdio>
#include <time.h>
int main(int argc, char *argv[])
{
	char operation = 0;
	time_t timeType = 0;
	time_t time1 = 0, time2 = 0;

	if (argc != 4) {
		printf("No arguments\n");
		return -1;
	}

	if (!strcmp(argv[1], "add") || !strcmp(argv[1], "sub")) {
		if (!strcmp(argv[1], "add")) operation = 'a';
		else operation = 's';
		if (!strcmp(argv[2], "min") || !strcmp(argv[2], "hour") || !strcmp(argv[2], "day") || !strcmp(argv[2], "sec")) {
			timeType = atoi(argv[3]);
			if (!strcmp(argv[2], "min")) timeType *= 60;
			else if (!strcmp(argv[2], "hour")) timeType *= 60 * 60;
			else if (!strcmp(argv[2], "day")) timeType *= 60 * 60 * 24;
		}
		else {
			printf("You gave wrong modifier\n");
			return -1;
		}
	}



	else if (!strcmp(argv[1], "diff")) {
		operation = 'd';
		for (int i = 2; i < argc; i++) {
			int hour = 0; int min = 0;
			int sec = 0;


			if (sscanf(argv[i], "%2d:%2d:%2d", &hour, &min, &sec) != 3 || strlen(argv[i]) != 8 || argv[i][7]<'0' || argv[i][7] > '9' || hour > 23 || min > 59 || sec > 59 || hour < 0 || min < 0 || sec < 0) {
				printf("Wrong format\n");
				return -1;
			}
			int timei = hour * 60 * 60 + min * 60 + sec;
			if (i == 2) time1 = timei;
			else time2 = timei;
		}
		if (!operation) {
			printf("You gave wrong or void operation\n");
			return -1;
		}

	}
		time_t timer = 0;
		struct tm *today = 0;
		switch (operation) {
		case 'a':
			time(&timer);
			timer += timeType;
			break;
		case 's':
			time(&timer);
			timer -= timeType;
			break;
		case 'd':
			timer = abs(time1 - time2);
			printf("%lld\n", timer);
			return 0;
		}

		today = localtime(&timer);
		printf("%s\n", asctime(today));

		return 0;
	
}