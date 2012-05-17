//
//  main.c
//  SINF1252_P3
//
//  Created by Ludovic Vannoorenberghe on 15/05/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include "extract.h"

const char * MARKER = "=--=--=--=--=--=--=--=--=--=#";
FILE * archive;
FILE * extractFile;

static void sig_handler(int signum) {
	if (signum == SIGTERM || signum == SIGINT) {
        fclose(archive);
        fclose(extractFile);
        exit(EXIT_SUCCESS);
    }
}


int main (int argc, char * argv[])
{
    int opt;
    int index = 0;
    if (argc < 2) {
        printf("Usage : ./extract [-n indice] filename" );
        exit(EXIT_FAILURE);
    }
    while ((opt = getopt(argc, argv, "n:")) != -1) {
        switch (opt) {
            case 'n':
                index = atoi(optarg);
                break;
            case '?':
                printf("Usage : ./extract [-n indice] filename");
                return 1;
            default:
                exit(EXIT_FAILURE);
        }
    }
    
    if (signal(SIGINT, sig_handler) == SIG_ERR) {
        perror("Erreur lors de l'enregistrement du signal");
        exit(EXIT_FAILURE);
    }
    
    if (signal(SIGTERM, sig_handler) == SIG_ERR) {
        perror("Erreur lors de l'enregistrement du signal");
        exit(EXIT_FAILURE);
    }
    
    char * filePath = argv[optind];
    if ((archive = fopen(filePath, "rb")) == NULL) {
        perror("Erreur lors de l'ouverture du fichier: ");
        printf("%s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
   
    char * newFilePath = malloc(sizeof(char)*strlen(filePath)-1);
    strncpy(newFilePath, filePath, (strlen(filePath)-2));
    if ((extractFile = fopen(newFilePath, "wb+")) == NULL) { 
        perror("Erreur lors de la creation du fichier");
        printf("%s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    int current = 0;
    while (current < index) {
        go_to_next_marker(archive, extractFile, 0);
        current++;
    }
    go_to_next_marker(archive, extractFile, 1);
    fclose(archive);
    fclose(extractFile);
    free(newFilePath);
    return EXIT_SUCCESS;
}

void go_to_next_marker(FILE * file, FILE * newFile, int copyMode) {
    char * nextLine = '\0';
    size_t lenghtLine;
    do {
        nextLine = fgetln(file, &lenghtLine);
        if (copyMode && strncmp(nextLine, MARKER, strlen(MARKER))) {
            fwrite(nextLine, sizeof(char), lenghtLine, newFile);
        }
    } 
    while (!feof(file) && strncmp(nextLine, MARKER, strlen(MARKER)));
}

/*int edit_file(FILE * oldFile, FILE * newFile, int line, char mode, int nbLine, char * pathFile) {
    char * name = tmpnam(NULL);
    FILE * tmp = fopen(name, "w+");
    int  c;
    int i;
    int l = 1;
    rewind(newFile);
    while (l < line) {
        c = fgetc(newFile);
        if (c == '\n') l++;
        fputc(c, tmp);
    }
    switch (mode) {
        case 'a':
            do {
                c = fgetc(newFile);
                fputc(c, tmp);
            } while (c != '\n');
            i = go_to_next_marker(oldFile);
            break;
        case 'c':
            do {
                c = fgetc(newFile);
            } while (c != '\n');
            i = go_to_next_marker(oldFile);
            break;
        case 'd':
            do {
                c = fgetc(newFile);
            } while (c != '\n');
            i = go_to_next_marker(oldFile);
        default:
            break;
    }
    do {
        c = fgetc(newFile);
        fputc(c, tmp);
    } while (c != EOF);
    rename(name, pathFile);
    fclose(tmp);
    return i;
}*/
