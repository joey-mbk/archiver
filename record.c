/*
*
* Record's header file
* Authors: Joey Moubarak, Ludovic Vannoorenberghe
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

int timer;							// time between each file change check
char *filepath;					// file to monitor
struct stat metadata;		// metadata of the file to monitor

static void sig_handler(int signum) {
	if (signum == SIGALRM) {
		char *msg = "Test\n";
		write(STDOUT_FILENO, msg, strlen(msg));
		alarm(3); // reset the timer
	}
	else if (signum == SIGINT) {
		char *msg = "Closing program\n";
		write(STDOUT_FILENO, msg, strlen(msg));
		exit(EXIT_SUCCESS);
	}
}

void start() {
	printf("%s\n", filepath);
	if (stat(filepath, &metadata) == -1) {
		perror("Error while retrieving metadata of file.");
		exit(EXIT_FAILURE);
	}
	printf("Last modif: %s", ctime(&metadata.st_mtime));
}

int main(int argc, char *argv[]) {	
	
	timer = 3; // TODO change this value to the param value
	filepath = argv[1]; // TODO change this value to the param value

	start(); // initiate the startup process

	/* register the different signal handlers */
	if (signal(SIGALRM, sig_handler) == SIG_ERR) {						// alarm handler
    perror("Error while registering SIGALRM callback function.");
    exit(EXIT_FAILURE);
  }
	if (signal(SIGINT, sig_handler) == SIG_ERR) {							// CTRL-C handler
    perror("Error while registering SIGINT callback function.");
    exit(EXIT_FAILURE);
  }

	alarm(timer);

	while (1); // run the program until a signal is sent

	exit(EXIT_SUCCESS);
}
