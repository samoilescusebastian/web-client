#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#define HEADER_TERMINATOR "\r\n\r\n"
#define HEADER_TERMINATOR_SIZE (sizeof(HEADER_TERMINATOR) - 1)
#define CONTENT_LENGTH "Content-Length: "
#define CONTENT_LENGTH_SIZE (sizeof(CONTENT_LENGTH) - 1)
#define BUFFLEN 4096

#include <vector>
#include <arpa/inet.h>
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <unistd.h>     /* read, write, close */

int establishConnection(uint32_t ip, int port, int flag);
uint32_t getIpAdress(const char *name);
void sendToServer(int sockfd, const char *message);
char *receiveFromServer(int sockfd);
void computeMessage(std::string& message, std::string line);
std::string computeGetRequest(std::string host, std::string url, std::string token,
                            std::vector<std::string> cookies);
std::string computePostRequest(std::string host, std::string url, std::string content_type, std::string token,
                           std::string payload, std::vector<std::string> cookies);
std::string computeDeleteRequest(std::string host, std::string url, std::string token);
#endif
