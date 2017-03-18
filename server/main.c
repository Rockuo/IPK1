
#include "../genericFunctions.h"
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#define R200 "OK"
#define R201 "Created"
#define R400 "Bad Request"
#define R403 "Forbidden"
#define R404 "Not Found"
#define R409 "Conflict"

int ls(char *path, char *response) {
    FILE *fp;
    int status, size = 100;
    char sm[size], *command = (char *) malloc(sizeof(char) * (strlen(path) + 4));
    response = (char *) malloc(sizeof(char) * size);
    sprintf(command, "ls %s", path);
    fp = popen(command, "r");
    if (fp == NULL) {
        return 404;
    }
    while (fgets(sm, size - 1, fp) != NULL) {
        printf("----%s", sm);
        size += 100;
        response = (char *) realloc(response, sizeof(char) * size);
        sprintf(response, "%s%s", response, sm);
    }
    status = pclose(fp);
    if (status == -1) {
        return 400;
    } else {
        return 200;
    }
}

char *createResponse(int code, char *responseBody) {
    char line1[40]; //memory waste
    char line2[100];
    char *line3 = "Content-Type: text/plain";
    char line4[100];
    char *line5 = "Content-Encoding: identity";
    sprintf(line1, "HTTP/1.1 %d %s", code,
            (code == 200 ? R200 :
             (code == 201 ? R201 :
              (code == 403 ? R403 :
               (code == 404 ? R404 :
                (code == 409 ? R409 : R400
                )
               )
              )
             )
            )
    );

    sprintf(line2, "Date: %s", currentTimeString());
    if (responseBody == NULL) {
        responseBody = (char *) malloc(sizeof(char *));
        bzero(responseBody, 1);
    }
    sprintf(line4, "Content-Length: %zu", strlen(responseBody));
    char *response = (char *) malloc(sizeof(char) * (strlen(responseBody) + 400));
    sprintf(response, "%s\n%s%s\n%s\n%s\n\n%s", line1, line2, line3, line4, line5, responseBody);
    return response;
}


int main(int argc, char *argv[]) {
    CLIENT_OR_SERVER = SERVER;
    int sockfd, newsockfd;
    uint16_t portno = 6677;
    struct sockaddr_in serv_addr, cli_addr;
    char *root = malloc(5 * sizeof(char));
    strcpy(root, "./");

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0) {
            root = argv[i + 1];
        } else if (strcmp(argv[i], "-p") == 0) {
            portno = (uint16_t)atoi(argv[i + 1]);
        }
    }


    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    listen(sockfd, 5);
    socklen_t clilen = sizeof(cli_addr);

    while ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen))) {
        if (newsockfd < 0) {
            perror("ERROR on accept");
            exit(1);
        }

        Header *header = getHeader(newsockfd);
        char *responseBody = NULL;
        int returnCode = 400;
        char *fullPath = (char *) malloc(sizeof(char) * (strlen(root) + strlen(header->path) + 1));


        sprintf(fullPath, "%s%s%s", root,header->path[0]=='/'?"":"/", header->path);

        if (strcmp("PUT", header->method) == 0) {
            if (strcmp("folder", header->type) == 0) {
                returnCode = mkdir(fullPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                if (returnCode < 0) {
                    switch (errno) {
                        case EACCES:
                        case EROFS:
                            returnCode = 403;
                            break;
                        case EEXIST:
                            returnCode = 409;
                            break;
                        default:
                            returnCode = 400;
                            break;
                    }
                } else {
                    returnCode = 200;
                }
            } else {
                ssize_t n;
                char *filestr = getBody(newsockfd, header->length, &n);
                if (n >= 0) {
                    FILE *fp = fopen(fullPath, "w");
                    if (fp == NULL) {
                        returnCode = 404;
                    } else {
                        fprintf(fp, "%s", filestr);
                        fclose(fp);
                        returnCode = 200;
                    }
                } else {
                    returnCode = 400;
                }
            }
        } else if (strcmp("GET", header->method) == 0) {
            if (strcmp("folder", header->type) == 0) {
                returnCode = ls(fullPath, responseBody);
            } else {
                responseBody = readFile(fullPath, &returnCode);
            }
        } else if (strcmp("DELETE", header->method) == 0) {
            if (strcmp("folder", header->type) == 0) {
                returnCode = rmdir(fullPath);
                if (returnCode < 0) {
                    switch (errno) {
                        case EACCES:
                        case EROFS:
                            returnCode = 403;
                        case EEXIST:
                        case ENOTEMPTY:
                            returnCode = 409;
                        default:
                            returnCode = 400;
                            break;
                    }
                } else {
                    returnCode = 200;
                }
            } else {
                returnCode = remove(fullPath);
                if (returnCode < 0) {
                    switch (errno) {
                        case EACCES:
                        case EROFS:
                            returnCode = 403;
                        case EEXIST:
                        case ENOTEMPTY:
                            returnCode = 409;
                        default:
                            returnCode = 400;
                            break;
                    }
                } else {
                    returnCode = 200;
                }
            }
        }

        char *serverResponse = createResponse(returnCode, responseBody);
        printf("%s", serverResponse);
        ssize_t n = send(newsockfd, serverResponse, strlen(serverResponse), 0);

        if (n < 0) {
            perror("ERROR writing to socket");
            exit(1);
        }
    }

    return 0;
}