#include <cstdio>
#include <sys/wait.h>
#include <sys/types.h> 
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	pid_t p; 
	pid_t pid = getpid(); 
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

	char *args[] = { argv[argc - 1], arg[0], NULL };
	p = fork();
	if (p && w) {
		wait(NULL);
		printf("\nCompleted!\n");
	}
	for (int i = 0; i < inst; i++) {
		if (!p) {
			p = fork();
			if (w) wait(NULL);
		}
	}
	if (p && getpid() != pid) execvp(argv[argc - 1], args);
}