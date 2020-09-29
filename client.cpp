#include<iostream>
#include <unordered_map>
#include "connection.h"
#include "message.h"
#include "helpers.h"


void registerAccount(std::string host, int socket) {
   
    std::string username;
    std::string password;
    std::string payload;
    std::string message;
    std::string response;

    int code;

    std::cout << "\n          --------------------\n";
    std::cout << "          REGISTER NEW ACCOUNT\n";
    std::cout << "          ---------------------\n\n";
    std::cout <<"           Insert an username\n\n";
    std::cout <<"           ---> ";
    std::getline(std::cin, username);
    std::cout <<"\n           Insert a password\n\n";
    std::cout <<"           ---> ";
    std::getline(std::cin, password);
    std::cout << "\n          --------------------\n";
    std::cout << "          REGISTER NEW ACCOUNT\n";
    std::cout << "          ---------------------\n\n";

    payload = computeAccountJson(username, password);

    message = computePostRequest(host, REGISTER_URL, std::string(), CONTENT_TYPE, 
                                 payload, std::vector<std::string>());
    sendToServer(socket, message.c_str());

    response = receiveFromServer(socket);
    code = extractCode(response.substr(0, response.find('\n')));

    switch (code)
    {
        case BAD_REQUEST:
            std::cout << "\n***The username " << username << " is taken.***\n";
            break;

        case CREATED:
            std::cout << "\n***Successfully created.***\n";
            break;

        default:
            std::cout << "ERROR:Invalid Response\n";
            break;
    }
}

bool authenticate(std::string host, std::vector<std::string>& cookies, int socket) {

    std::string username;
    std::string password;
    std::string message;
    std::string payload;
    std::string response;

    int code;

    std::cout << "\n          --------------------\n";
    std::cout << "                  LOGIN\n";
    std::cout << "          ---------------------\n\n";
    std::cout <<"             Insert username\n\n";
    std::cout <<"           ---> ";
    std::getline(std::cin, username);
    std::cout <<"\n             Insert password\n\n";
    std::cout <<"           ---> ";
    std::getline(std::cin, password);
    std::cout << "\n          --------------------\n";
    std::cout << "                  LOGIN\n";
    std::cout << "          ---------------------\n\n";

    payload = computeAccountJson(username, password);

    message = computePostRequest(host, LOGIN_URL, std::string(), CONTENT_TYPE, payload, cookies);
    sendToServer(socket, message.c_str());

    response = receiveFromServer(socket);
    code = extractCode(response.substr(0, response.find('\n')));

    switch (code)
    {
        case BAD_REQUEST:
            std::cout << "\n***Invalid credentials. Try others.***\n";
            break;

        case SUCCESS:
            extractCookies(cookies, response);
            std::cout << "\n***Successfully logged in.***\n";
            return true;
            break;

        default:
            std::cout << "ERROR:Invalid Response\n";
            break;
    }
    return false;
}

void accesLibrary(std::string host, std::vector<std::string>& cookies, int socket, std::string& token) {
    
    std::string message;
    std::string response;

    int code;

    message = computeGetRequest(host, ACCESS_LIBRARY, std::string(), cookies);
    std :: cout << message << "\n";
    sendToServer(socket, message.c_str());

    response = receiveFromServer(socket);
    code = extractCode(response);

    switch (code)
    {
        case BAD_REQUEST:
            std::cout << "\n***You are not logged in***\n";
            break;

        case SUCCESS:
            std::cout << "\n***Successfully subscribed to library.***\n";
            token = extractToken(response);
            break;

        default:
            std::cout << "ERROR:Invalid Response\n";
            break;
    }
}

void getBooksInfos(std::string host, std::string token, int socket) {

    std::string message;
    std::string response;

    int code;

    std::vector<json> books;

    message = computeGetRequest(host, BOOKS_URL, token, std::vector<std::string>());
    sendToServer(socket, message.c_str());

    response = receiveFromServer(socket);
    code = extractCode(response);


    switch (code)
    {
        case FORBIDDEN:
            std::cout << "\n***You are not authorized to enter in library***\n";
            break;

        case SUCCESS:
            extractBooks(response, books);
            std::cout << "\n          --------------------\n";
            std::cout << "                BOOKS INFO\n";
            std::cout << "          ---------------------\n";

            for (size_t i = 0; i < books.size(); i++) {
                std::cout << "\n              BOOK " << i << "\n\n";
                std::cout << "         Id:" << books[i]["id"] << "\n";
                std::cout << "         Title:" << books[i]["title"] << "\n";
            }

            std::cout << "\n          --------------------\n";
            std::cout << "                BOOKS INFO\n";
            std::cout << "          ---------------------\n\n";
            break;

        default:
            std::cout << "ERROR:Invalid Response\n";
            break;
    }
}

void getBook(std::string host, std::string token, int socket) {

    std::string message;
    std::string response;
    std::string id;

    int code;

    json book;

    std::cout <<"\n           Insert book id\n\n";
    std::cout <<"           ---> ";
    std::getline(std::cin, id);
    if (!isNumeric(id) || std::stoi(id) < 0) {
        std :: cout << "Invalid id!\n";
        return;
    }

    std::string getBookUrl = std::string(BOOKS_URL) + "/" + id;
    message = computeGetRequest(host, getBookUrl, token, std::vector<std::string>());
    sendToServer(socket, message.c_str());

    response = receiveFromServer(socket);
    code = extractCode(response);

    switch (code)
    {
        case FORBIDDEN:
            std::cout << "\n***You are not authorized to enter in library.***\n";
            break;

        case SUCCESS:
            book = extractBook(response);
            std::cout << "\n          --------------------\n";
            std::cout << "            BOOK DESCRIPTION\n";
            std::cout << "          ---------------------\n\n";

            std::cout << "          Title: " << book["title"] << "\n";
            std::cout << "          Author: " << book["author"] << "\n";
            std::cout << "          Genre: " << book["genre"] << "\n";
            std::cout << "          Page Count: " << book["page_count"] << "\n";
            std::cout << "          Publisher: " << book["publisher"] << "\n";

            std::cout << "\n          --------------------\n";
            std::cout << "             BOOK DESCRIPTION\n";
            std::cout << "          ---------------------\n\n";
            break;
        
        case NOT_FOUND:
            std::cout << "\n***No book was found.***\n";
            break;
        default:
            std::cout << "ERROR:Invalid Response\n";
            break;
    }
}

void deleteBook(std::string host, std::string token, int socket) {

    std::string message;
    std::string response;
    std::string id;

    int code;

    std::cout <<"\n           Insert book id\n\n";
    std::cout <<"           ---> ";
    std::getline(std::cin, id);
    if (!isNumeric(id) || std::stoi(id) < 0) {
        std :: cout << "Invalid id!\n";
        return;
    }

    std::string getBookUrl = std::string(BOOKS_URL) + "/" + id;
    message = computeDeleteRequest(host, getBookUrl, token);
    sendToServer(socket, message.c_str());

    response = receiveFromServer(socket);
    code = extractCode(response);

    switch (code)
    {
        case FORBIDDEN:
            std::cout << "\n***You are not authorized to enter in library.***\n";
            break;

        case SUCCESS:
            std::cout << "\n***Successfully deleted from library.\n";
            break;

        case NOT_FOUND:
            std::cout << "\n***No book was found.***\n";
            break;
        default:
            std::cout << "ERROR:Invalid Response\n";
            break;
    }

}

void addBook(std::string host, std::string token, int socket) {

    std::string title;
    std::string author;
    std::string genre;
    std::string pageCount;
    std::string publisher;
    std::string message;
    std::string payload;
    std::string response;

    int code;

    std::cout << "\n          --------------------\n";
    std::cout << "                ADD BOOK\n";
    std::cout << "          ---------------------\n\n";
    std::cout <<"             Insert title\n\n";
    std::cout <<"           ---> ";
    std::getline(std::cin, title);
    std::cout <<"\n             Insert author\n\n";
    std::cout <<"           ---> ";
    std::getline(std::cin, author);
    std::cout <<"\n             Insert genre\n\n";
    std::cout <<"           ---> ";
    std::getline(std::cin, genre);
    std::cout <<"\n             Insert page_count\n\n";
    std::cout <<"           ---> ";
    std::getline(std::cin, pageCount);
    std::cout <<"\n             Insert publisher\n\n";
    std::cout <<"           ---> ";
    std::getline(std::cin, publisher);
    std::cout << "\n          --------------------\n";
    std::cout << "                ADD BOOK\n";
    std::cout << "          ---------------------\n\n";

    if (!validFormat(title, author, genre, pageCount, publisher)) {
        std::cout << "Invalid data format!\n";
    }

    payload = computeBookJson(title, author, genre, pageCount, publisher);

    message = computePostRequest(host, BOOKS_URL, token, CONTENT_TYPE,
                                payload, std::vector<std::string>());
    sendToServer(socket, message.c_str());

    response = receiveFromServer(socket);
    code = extractCode(response.substr(0, response.find('\n')));
    
    switch (code)
    {   
        case FORBIDDEN:
            std::cout << "\n***You are not authorized to enter in library.***\n";
            break;

        case SUCCESS:
            std::cout << "\n***Successfully added in library.***\n";
            break;

        default:
            std::cout << "ERROR:Invalid Response\n";
            break;
    }
}

void logout(std::string host, std::vector<std::string>& cookies, int socket) {

    std::string message;
    std::string response;

    int code;

    message = computeGetRequest(host, LOGOUT_URL, std::string(), cookies);
    sendToServer(socket, message.c_str());

    response = receiveFromServer(socket);
    code = extractCode(response.substr(0, response.find('\n')));
    switch (code)
    {
        case BAD_REQUEST:
            std::cout << "\n***You are not logged in.***\n";
            break;

        case SUCCESS:
            std::cout << "\n***Successfully logged out.***\n";
            break;

        default:
            std::cout << "ERROR:Invalid Response\n";
            break;
    }
}

void buildEncodings(std::unordered_map<std::string, int>& encodingMap) {

    encodingMap["register"] = 0;
    encodingMap["login"] = 1;
    encodingMap["enter_library"] = 2;
    encodingMap["get_books"] = 3;
    encodingMap["get_book"] = 4;
    encodingMap["add_book"] = 5;
    encodingMap["delete_book"] = 6;
    encodingMap["logout"] = 7;
    encodingMap["exit"] = 8;

}

int main() {

    uint32_t ip;
    ip = getIpAdress(HOST);
    std::string message;
    
    int socket;
    
    std::unordered_map<std::string, int> encodingMap;
    buildEncodings(encodingMap);
    std::vector<std::string> cookies;
    bool login = false;
    std::string fromStdin;
    std::string token;
    int code;
    while(true) {
        std::cout << "\nInsert a new command...\n\n--> ";
        std::getline(std::cin, fromStdin);
        if (encodingMap.find(fromStdin) == encodingMap.end()) {
            std::cout << "Invalid command! Try again.\n";
            continue;
        }
        code = encodingMap[fromStdin];
        socket = establishConnection(ip, PORT, 0);
        switch (code)
        {
        case 0:
            registerAccount(HOST, socket);
            break;
        
        case 1:
            if (!login) {
                login = authenticate(HOST, cookies, socket);
                
            } else {
                std::cout << "You are already logged in.\n";
            }
            
            break;

        case 2:
            accesLibrary(HOST, cookies, socket, token);
            break;

        case 3:
            getBooksInfos(HOST, token, socket);
            break;

        case 4:
            getBook(HOST, token, socket);
            break;

        case 5:
            addBook(HOST, token, socket);
            break;

        case 6:
            deleteBook(HOST, token, socket);
            break;
        
        case 7:
            logout(HOST, cookies, socket);
            cookies.clear();
            token = "";
            login = false;
            break;

        case 8:
            close(socket);
            return 0;
        default:
            break;
        }

    }
    return 0;
}