#include "../genericFunctions.h"

#define FILEn "file"
#define FOLDER "folder"

int sendRequest(char *serverName, uint16_t port, char *content);

void parsePath(char **path, char **server, char **port);

char *generateHeader(actionType action, char *serverName, char *path, size_t bodyLength, char *encode, char *type);

int main(int argc, char *argv[]) {
    CLIENT_OR_SERVER = CLIENT;

    if (argc < 3 || argc > 4) {
        fprintf(stderr, "Invalid number of args");
        exit(EXIT_FAILURE);
    }

    char *action, *path, *server, *port, *what, *content = malloc(1);
    bool isFile = true;
    content[0] = '\0';

    action = argv[1];
    path = argv[2];

    actionType aType;
    parsePath(&path, &server, &port);

    if (strcmp(action, "del") == 0) {
        aType = ACTION_DEL;
    } else if (strcmp(action, "get") == 0) {
        aType = ACTION_GET;
    } else if (strcmp(action, "put") == 0) {
        if (argc != 4) {
            exit(123);
        }
        char *filename = (char *) malloc(sizeof(char *) * (strlen(argv[3]) + 1));
        strcpy(filename, argv[3]);
        int n;
        content = readFile(filename, &n);
        if (n == 404) {
            fprintf(stderr, "Not a file.");
            exit(EXIT_FAILURE);
        }
        aType = ACTION_PUT;
    } else if (strcmp(action, "lst") == 0) {
        aType = ACTION_LST;
        isFile = false;
    } else if (strcmp(action, "mkd") == 0) {
        aType = ACTION_MKD;
        isFile = false;
    } else if (strcmp(action, "rmd") == 0) {
        aType = ACTION_RMD;
        isFile = false;
    } else {
        fprintf(stderr, "Unnown action %s", action);
        exit(EXIT_FAILURE);
    }
    size_t contentLength = aType != ACTION_PUT ? 0 : strlen(content);
    char *header = generateHeader(aType, server, path, contentLength, "identity", isFile ? FILEn : FOLDER);
    char *request = malloc(sizeof(char) * (contentLength + strlen(header)));
    sprintf(request, "%s\n%s", header, content);
    //todo check port
    int sockfd = sendRequest(server, (uint16_t) atoi(port), request);
    Header *responseHeader = getHeader(sockfd);
    ssize_t responses = 0;
    if (aType == ACTION_DEL) {

    } else if (aType == ACTION_GET) {
        if(responseHeader->respCode == 200) {
            char *saveTo;
            if (argc == 4) {
                saveTo = argv[3];
            } else {
                char *filename = getFilenameFromPath(path);
                saveTo = malloc(sizeof(char) * (strlen(filename) + 5));
                sprintf(saveTo, "./%s", filename);
            };

            FILE *fp = fopen(saveTo, "w");
            if (fp == NULL) {
                fprintf(stdout, "\"Unknown error.");
                exit(EXIT_FAILURE);
            } else {
                ssize_t a;
                fprintf(fp, "%s", getBody(sockfd, responseHeader->length, &a));
                fclose(fp);
                return 0;
            }
        }
    } else if (aType == ACTION_PUT) {

    } else if (aType == ACTION_LST) {
        if (responseHeader->respCode == 200)
            printf("%s", getBody(sockfd, responseHeader->length, &responses));
    } else if (aType == ACTION_MKD) {

    } else if (aType == ACTION_RMD) {

    }
    if (responseHeader->respCode == 200) {
        return 0;
    }
    fprintf(stdout, "\"Unknown error.");
    exit(EXIT_FAILURE);
}

void parsePath(char **path, char **server, char **port) {
    if (sscanf((*path), "http://%s", (*path)) != 1) {
        fprintf(stderr, "Wrong path");
        exit(1);
    }
    *server = strtok((*path), ":");
    *port = strtok(NULL, "/");
    *path = strtok(NULL, "\0");
    char *slashedPath = malloc(sizeof(char) * (strlen(*path) + 1));
    sprintf(slashedPath, "/%s", *path);
    *path = slashedPath;
}


int sendRequest(char *serverName, uint16_t port, char *content) {
    int sockfd;
    ssize_t sendCount;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    /* Create a socket point */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    server = gethostbyname(serverName);

    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, (size_t) server->h_length);
    serv_addr.sin_port = htons(port);

    /* Now connect to the server */
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        exit(1);
    }

    /* Now ask for a message from the user, this message
       * will be read by server
    */

    sendCount = send(sockfd, content, strlen(content), 0);
    if (sendCount < 0)
        fprintf(stderr, "ERROR: sendto");

    return sockfd;
}

char *generateHeader(actionType action, char *serverName, char *path, size_t bodyLength, char *encode, char *type) {
    const char *http = "HTTP/1.1";
    const char *host = "Host:";
    const char *date = "Date:";
    const char *acceptFull = "Accept: text/plain";
    const char *encodeNm = "Accept-Encoding:";
    const char *contentTypeFull = "Content-Type: application/octet-stream";
    const char *cLength = "Content-Length:";
    char *get = "GET";
    char *put = "PUT";
    char *del = "DELETE";
    char *dateTime = currentTimeString();
    size_t length = strlen(http) + strlen(host) + strlen(date) + strlen(acceptFull) + strlen(encodeNm) +
                    strlen(contentTypeFull) + strlen(cLength) + strlen(serverName) + strlen(path) + strlen(dateTime) +
                    strlen(encode);
    char *act;
    switch (action) {
        case ACTION_MKD:
            act = put;
            break;
        case ACTION_RMD:
            act = del;
            break;
        case ACTION_DEL:
            act = del;
            break;
        case ACTION_GET:
            act = get;
            break;
        case ACTION_LST:
            act = get;
            break;
        case ACTION_PUT:
            act = put;
            break;
        default:
            act = NULL;
    }
    length += strlen(act);
    char bodyLengthStr[50];
    sprintf(bodyLengthStr, "%zu", bodyLength);
    length += strlen(bodyLengthStr);
    length += 20; //safety length (spaces and \n)
    char *head = malloc(sizeof(char) * length);
    sprintf(head, "%s %s?type=%s %s\n%s %s\n%s %s%s\n%s %s\n%s\n%s %zu\n", act, path, type, http, host, serverName,
            date, dateTime,
            acceptFull, encodeNm, encode, contentTypeFull, cLength, bodyLength);
    return head;
}
