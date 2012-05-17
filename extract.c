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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "extract.h"

char * MARKER = "=--=--=--=--=--=--=--=--=--=#";

int main (int argc, char * argv[])
{
    int opt;
    int index;
    if (argc < 2) {
        printf("Usage : -n indice filename" );
        exit(EXIT_FAILURE);
    }
    while ((opt = getopt(argc, argv, "n:")) != -1) {
        switch (opt) {
            case 'n':
                index = atoi(optarg);
                break;
            case '?':
                printf("bad");
                return 1;
            default:
                exit(EXIT_FAILURE);
        }
    }
    FILE * file;
    char * filePath = argv[optind];
    if ((file = fopen(filePath, "r")) < 0) perror("Erreur lors de l'ouverture du fichier");
    FILE * newFile;
    char * newFilePath = malloc(strlen(filePath)-1);
    strncpy(newFilePath, filePath, (strlen(filePath)-2));
    if ((newFile = fopen(newFilePath, "w+")) < 0) perror("Erreur lors de la creation du fichier");
    int current = 0;
    while (current < 0) {
        go_to_next_marker(file, newFile, 0);
    }
    go_to_next_marker(file, newFile, 1);
    fclose(file);
    fclose(newFile);
    free(newFilePath);
    return 0;
}

void go_to_next_marker(FILE * file, FILE * newFile, int copyMode) {
    char * nextLine = '\0';
    size_t * lenghtLine;
    do {
        nextLine = fgetln(file, lenghtLine);
       
    } 
    while (strncmp(nextLine, marker, strlen(marker)) );
}

void replace_line(FILE * file, int i, char * newLine) {
    
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

long int go_to_line(FILE * file, int i, fpos_t * pos) {
    rewind(file);
    for(int a = 1; a < i; a++) {
        fscanf(file, "%*[^\n]");
    }
    fscanf(file, "%*c");
    return ftell(file);
}