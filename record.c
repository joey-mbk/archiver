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

static void sig_handler(int signum) {
	char *msg = "Test\n";
  write(STDOUT_FILENO, msg, strlen(msg));
}

int main(int argc, char *argv[]) {

	if (signal(SIGALRM, sig_handler) == SIG_ERR) {
    perror("Error while registering callback function.");
    exit(EXIT_FAILURE);
  }

	alarm(3);

	exit(EXIT_SUCCESS);
}
