#ifndef _HELPERS_H
#define _HELPERS_H
#define PORT 8080
#define HOST "ec2-3-8-116-10.eu-west-2.compute.amazonaws.com"
#define BAD_REQUEST 400
#define CREATED 201
#define SUCCESS 200
#define FORBIDDEN 403
#define NOT_FOUND 404
#define NPOS -1
#define CONTENT_TYPE "application/json"
#define REGISTER_URL "/api/v1/tema/auth/register"
#define LOGIN_URL "/api/v1/tema/auth/login"
#define ACCESS_LIBRARY "/api/v1/tema/library/access"
#define BOOKS_URL "/api/v1/tema/library/books"
#define LOGOUT_URL "/api/v1/tema/auth/logout"


#define DIE(assertion, call_description)                                       \
  do {                                                                         \
    if (assertion) {                                                           \
      fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__);                       \
      perror(call_description);                                                \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

#endif