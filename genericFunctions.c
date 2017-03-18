//
// Created by rockuo on 6.3.17.
//

#include "genericFunctions.h"

char *getFilenameFromPath(char *path) {
    char *last = NULL, *temp = malloc(sizeof(char*)*(strlen(path)+1));
    strcpy(temp,path);
    char *fuck = strtok(temp, "/");
    while (fuck != NULL) {
        last = fuck;
        fuck = strtok(NULL, "/");
    }
    return last;
}

char *readFile(char *filename, int *n) {
    char *buffer = NULL;
    size_t length;
    FILE *fp = fopen(filename, "rb");

    if (fp != NULL) {
        fseek(fp, 0, SEEK_END);
        length = (size_t)ftell(fp);
        fseek(fp, 0, SEEK_SET);
        buffer = malloc(sizeof(char*)*length);
        if (buffer) {
            fread(buffer, 1, length, fp);
        }
        fclose(fp);
    } else {
        *n = 404;
        return "";
    }
    *n = 200;
    return buffer;
}

Header *getHeader(int socket) {
    char item[2];
    int size = 100;
    char *response = malloc(sizeof(char) * size);
    int last = '\0';
    response[0] = '\0';
    response[1] = '\0';
    ssize_t numberRecv;

    while (true) {
        bzero(item,2);
        numberRecv = recv(socket, item, 1, 0);
        if (numberRecv < 0) {
            printf(response);
            fprintf(stderr, "ERROR: recvfrom");
            exit(EXIT_FAILURE);
        }
        if (strlen(response) > size - 10) {
            size += 100;
            response = realloc(response, sizeof(char) * size);
        }
        response[strlen(response)] = item[0];
        response[strlen(response) + 1] = '\0';
        if (last == '\n' && item[0] == '\n') {
            break;
        }
        last = item[0];
    }
    return parseHeader(response);
}

void processHeaderPart(char *line, Header *header) {
    char *key = strtok(line, ": ");
    char *value = strtok(NULL, "\0");

    if (strcmp("Host", key) == 0) {
        header->host = value;
    } else if (strcmp("Date", key) == 0) {
        header->date = value;
    } else if (strcmp("Accept", key) == 0) {
        header->accept = value;
    } else if (strcmp("Accept-Encoding", key) == 0) {
        header->encode = value;
    } else if (strcmp("Content-Length", key) == 0) {
        sscanf(value, "%zu", &(header->length));
    }
}

Header *parseHeader(char *headerStr) {
    Header *header = (Header *) malloc(sizeof(Header));
    char *line;
    line = strtok(headerStr, "\n");
    headerStr = strtok(NULL, "\0");
    if (CLIENT_OR_SERVER == SERVER) {
        header->method = strtok(line, " ");
        header->path = strtok(NULL, "?");
        strtok(NULL, "=");
        header->type = strtok(NULL, " ");
    } else {
        strtok(line, " ");
        char *respCode = strtok(NULL, " ");
        for (int i = 0; i < strlen(respCode); i++) {
            if (!isdigit(respCode[i])) {
                exit(1234);
            }
        }
        header->respCode = (size_t) atoi(respCode);
    }
    line = strtok(headerStr, "\n");
    while (true) {
        headerStr = strtok(NULL, "\0");
        processHeaderPart(line, header);
        line = strtok(headerStr, "\n");
        if (line == NULL) {
            break;
        }
    }
    return header;
}

char *getBody(int socket, size_t length, ssize_t *n) {
    if(length == 0){
        return "";
    }
    char *body = (char *) malloc(sizeof(char) * (length + 1));
    *n = recv(socket, body, length, 0);
    return body;
}

char *currentTimeString() {
    time_t dateTime = time(NULL);
    char *dateTimeString;

    if (dateTime == ((time_t) -1)) {
        fprintf(stderr, "Failure to obtain the current time.\n");
        exit(EXIT_FAILURE);
    }

    dateTimeString = ctime(&dateTime);
    if (dateTimeString == NULL) {
        (void) fprintf(stderr, "Failure to convert the current time.\n");
        exit(EXIT_FAILURE);
    }
    return dateTimeString;
}


