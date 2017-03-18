//
// Created by rockuo on 5.3.17.
//

#ifndef IPK1_STRUCTURES_H
#define IPK1_STRUCTURES_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define CLIENT 1
#define SERVER 2
int CLIENT_OR_SERVER;
typedef enum actionTypeEnum {
    ACTION_DEL = 1000,
    ACTION_GET = 1001,
    ACTION_PUT = 1002,
    ACTION_LST = 1003,
    ACTION_MKD = 1004,
    ACTION_RMD = 1005

} actionType;

typedef struct headerStruct{
    char *method;
    char *path;
    char *host;
    char *date;
    char *accept;
    char *encode;
    char *type;
    size_t length;
    size_t respCode;

} Header;

#endif //IPK1_STRUCTURES_H
