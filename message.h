#ifndef _MESSAGE_H_
#define _MESSAGE_H_
#include <iostream>
#include <sstream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

std::string computeAccountJson(std::string username, std::string password);
std::string computeBookJson(std::string title, std::string author, std::string genre,
                            std::string pageCount, std::string publisher);
int extractCode(std::string response);
void extractCookies(std::vector<std::string>& cookies, std::string response);
std::string extractToken(std::string response);
bool validFormat(std::string title, std::string author, std::string genre,
                 std::string pageCount, std::string publisher);
void extractBooks(std::string response, std::vector<json>& books);
json extractBook(std::string response);
bool isNumeric(std::string numeric);
#endif // !_MESSAGE_H_
