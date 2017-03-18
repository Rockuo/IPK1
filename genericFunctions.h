//
// Created by rockuo on 6.3.17.
//

#ifndef IPK1_GENERICFUNCTIONS_H
#define IPK1_GENERICFUNCTIONS_H

#include "structures.h"

Header *getHeader(int socket);
Header *parseHeader(char *headerStr);
char *readFile(char *filename, int *n);
char *getBody(int socket, size_t length, ssize_t *n);
char *getFilenameFromPath(char *path);
char *currentTimeString();


#endif //IPK1_GENERICFUNCTIONS_H
