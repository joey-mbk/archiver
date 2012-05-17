//
//  extract.h
//  SINF1252_P3
//
//  Created by Ludovic Vannoorenberghe on 15/05/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef SINF1252_P3_extract_h
#define SINF1252_P3_extract_h

void go_to_next_marker(FILE * file, FILE * newFile, int copyMode);
long int go_to_line(FILE * file, int i, fpos_t * pos);
int edit_file(FILE * oldFile, FILE * newFile, int line, char mode, int nbLine, char * pathFile);
#endif
