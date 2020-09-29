#include <iostream>
#include "connection.h"
#include "buffer.h"
#include "helpers.h"

int establishConnection(uint32_t ip, int port, int flag) {

  struct sockaddr_in serv_addr;
  int sockfd = socket(AF_INET, SOCK_STREAM, flag);
  DIE(sockfd < 0, "ERROR: Failed to create socket.\n");

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);
  serv_addr.sin_addr.s_addr = ip;

  DIE(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0,
      "ERROR: Failed to connect to server.\n");

  return sockfd;

}

uint32_t getIpAdress(const char *name) {

  struct hostent *hostEntry;
  hostEntry = gethostbyname(name);
  DIE(hostEntry == nullptr, "Error: invalid host name address.\n");

  return ((struct in_addr *)hostEntry->h_addr_list[0])->s_addr;

}

void sendToServer(int sockfd, const char *message) {

  int bytes, sent = 0;
  int total = strlen(message);

  do {
    bytes = write(sockfd, message + sent, total - sent);
    DIE(bytes < 0, "ERROR: Failed to write to server.\n");

    if (bytes == 0) {
      break;
    }

    sent += bytes;
  } while (sent < total);

}

char *receiveFromServer(int sockfd) {

  char response[BUFFLEN];
  buffer buffer = buffer_init();
  int header_end = 0;
  int content_length = 0;

  do {
    int bytes = read(sockfd, response, BUFFLEN);

    DIE(bytes < 0, "Failed to read response from server.\n");

    if (bytes == 0) {
      break;
    }

    buffer_add(&buffer, response, (size_t)bytes);

    header_end =
        buffer_find(&buffer, HEADER_TERMINATOR, HEADER_TERMINATOR_SIZE);

    if (header_end >= 0) {
      header_end += HEADER_TERMINATOR_SIZE;

      int content_length_start =
          buffer_find_insensitive(&buffer, CONTENT_LENGTH, CONTENT_LENGTH_SIZE);

      if (content_length_start < 0) {
        continue;
      }

      content_length_start += CONTENT_LENGTH_SIZE;
      content_length = strtol(buffer.data + content_length_start, NULL, 10);
      break;
    }
  } while (1);
  size_t total = content_length + (size_t)header_end;

  while (buffer.size < total) {
    int bytes = read(sockfd, response, BUFFLEN);

    DIE(bytes < 0, "Failed to read response from server.\n");

    if (bytes == 0) {
      break;
    }

    buffer_add(&buffer, response, (size_t)bytes);
  }
  buffer_add(&buffer, "", 1);
  return buffer.data;

}

void computeMessage(std::string& message, std::string line)
{
    line += "\r\n";
    message += line;
}

std::string computeGetRequest(std::string host, std::string url, std::string token,
                            std::vector<std::string> cookies)
{
    std::string message;
    std::string line;

    line = "GET " + url + " HTTP/1.1";
    
    computeMessage(message, line);
    
    line = "Host: " + host;
    computeMessage(message, line);
    
    if (token != "") {
      line = "Authorization: Bearer " + token;
      computeMessage(message, line);
    }

    if (cookies.size() > 0) {
      line = "Cookie:";
      size_t i;
      for (i = 0; i < cookies.size() - 1; i++) {
        line += " ";
        line += cookies[i];
        line += ";";
      }
      line += " ";
      line += cookies[i];
      computeMessage(message, line);
    }
    computeMessage(message, "");
    return message;
}

std::string computeDeleteRequest(std::string host, std::string url, std::string token)
{
    std::string message;
    std::string line;

    line = "DELETE " + url + " HTTP/1.1";
    
    computeMessage(message, line);
    
    line = "Host: " + host;
    computeMessage(message, line);
    
    if (token != "") {
      line = "Authorization: Bearer " + token;
      computeMessage(message, line);
    }

    computeMessage(message, "");
    return message;
}
std::string computePostRequest(std::string host, std::string url, std::string token, 
                              std::string content_type, std::string payload, std::vector<std::string> cookies) { 

    std::string message;
    std::string line;

    line = "POST " + url + " HTTP/1.1";
    computeMessage(message, line);
    
    line = "Host: " + host;
    computeMessage(message, line);

    if (token != "") {
      line = "Authorization: Bearer " + token;
      computeMessage(message, line);
    }
    

    line = "Content-Type: ";
    line += content_type;
    computeMessage(message, line);

    line = "Content-Length: " + std::to_string(payload.size());
    computeMessage(message, line);
    if (cookies.size() > 0) {
      line = "Cookie:";
      size_t i;
      for (i = 0; i < cookies.size() - 1; i++) {
        line += " ";
        line += cookies[i];
        line += ";";
      }
      line += " ";
      line += cookies[i];
      computeMessage(message, line);
    }

    computeMessage(message, "");

    message += payload;

    return message;
}