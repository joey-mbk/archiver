//
//  extract.h
//  SINF1252_P3
//
//  Authors: Joey Moubarak, Ludovic Vannoorenberghe
//

#ifndef SINF1252_P3_extract_h
#define SINF1252_P3_extract_h

void go_to_next_marker(FILE * file, FILE * newFile, int copyMode);
int edit_file(FILE * oldFile, FILE * newFile, int line, char mode, int nbLine);
#endif
