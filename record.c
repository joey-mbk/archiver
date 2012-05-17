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
#include <errno.h>
#include <utime.h>

int timer;							// time between each file change check
char *filepath;					// file to monitor
struct stat mdata;			// metadata of the file to monitor
struct stat prvmdata;		// previous metadata of the file to monitor
struct stat archmdata;	// metadata of the archive
FILE *file;							// the file
FILE *archive;					// the archive
char *archivepath;			// archive containing the versions
int verlimit;						// max number of versions to keep
int vernb;							// current count of versions

void record() {
	printf("Recording...\n");

	/* reload the monitored file in read mode */
	file = fopen(filepath, "r");
	if (file == NULL) {
		perror("Error while opening monitored file in read mode.");
		exit(EXIT_FAILURE);
	}

	archive = fopen(archivepath, "a"); // opening the archive in write at end of text file mode

	char *str = malloc(sizeof(char)*64);
	if (str == NULL)
		perror("Error while allocating str");

	while (!feof(file)) {
		fgets(str, 64, file);
		fputs(str, archive);
	}
	fputs("\n--=--=--=--=--=--=--=--=--=#\n", archive);
}

void start() {
	printf("%s\n", filepath);
	printf("%s\n", archivepath);
	
	/* get the metadata of the monitored file */
	if (stat(filepath, &mdata) == -1) {
		perror("Error while retrieving metadata of monitored file.");
		exit(EXIT_FAILURE);
	}
	
	/* open the monitored file in read mode */
	file = fopen(filepath, "r");
	if (file == NULL) {
		perror("Error while opening monitored file in read mode.");
		exit(EXIT_FAILURE);
	}

	archive = fopen(archivepath, "r"); // opening the archive in read mode

	if (stat(archivepath, &archmdata) == -1) {
		perror("Error while retrieving metadata of the archive.");
		exit(EXIT_FAILURE);
	}

	if (archmdata.st_mtime != mdata.st_mtime) {
		printf("A modification was detected since last record.\n");
		record();
	}

	printf("Last modif: %s", ctime(&mdata.st_mtime));
}

static void sig_handler(int signum) {
	if (signum == SIGALRM) {
		char *msg = "Alarm\n";
		write(STDOUT_FILENO, msg, strlen(msg));
	
		prvmdata = mdata; // saving the metadata
		
		if (stat(filepath, &mdata) == -1) {
			perror("Error while retrieving metadata of monitored file.");
			exit(EXIT_FAILURE);
		}

		if (mdata.st_mtime == prvmdata.st_mtime)
			printf("No modification.\n");
		else {
			printf("Modification detected!\n");
			record();
		}

		alarm(3); // reset the timer
	}
	else if (signum == SIGINT) {
		if (fclose(file) || fclose(archive)) {
			perror("Error while closing streams");
			exit(EXIT_FAILURE);
		}

		/* setting same modification time of file on archive */
		if (stat(filepath, &mdata) == -1) {
			perror("Error while retrieving metadata of monitored file.");
			exit(EXIT_FAILURE);
		}
		struct utimbuf modiftime;
		modiftime.modtime = mdata.st_mtime;
		utime(archivepath, &modiftime);

		char *msg = "Closing program\n";
		write(STDOUT_FILENO, msg, strlen(msg));
		exit(EXIT_SUCCESS);
	}
}

int main(int argc, char *argv[]) {	
	
	timer = 3; // TODO change this value to the param value
	filepath = argv[1]; // TODO change this value to the param value
	archivepath = argv[2]; // TODO change this value to the param value

	start(); // initiate the startup process

	/* register the different signal handlers */
	if (signal(SIGALRM, sig_handler) == SIG_ERR) {	// alarm handler
    perror("Error while registering SIGALRM callback function.");
    exit(EXIT_FAILURE);
  }
	if (signal(SIGINT, sig_handler) == SIG_ERR) {		// CTRL-C handler
    perror("Error while registering SIGINT callback function.");
    exit(EXIT_FAILURE);
  }

	alarm(timer);

	while (1); // run the program until a signal is sent

	exit(EXIT_SUCCESS);
}
