#include "message.h"
#include "helpers.h"

// construire payload pentru un cont nou
std::string computeAccountJson(std::string username, std::string password) {
    
    json registerJson;
    registerJson["username"] = username;
    registerJson["password"] = password;
    
    return registerJson.dump();
}

// construire payload pentru o noua carte
std::string computeBookJson(std::string title, std::string author, std::string genre,
                            std::string pageCount, std::string publisher) {
    json bookJson;
    bookJson["title"] = title;
    bookJson["author"] = author;
    bookJson["genre"] = genre;
    bookJson["page_count"] = std::stoi(pageCount);
    bookJson["publisher"] = publisher;

    return bookJson.dump();
}

// extragere cod din raspuns HTTP
int extractCode(std::string response) {
    
    std::stringstream stringStream;
    stringStream << response;

    std::string word;
    int code;

    while (!stringStream.eof()) {

        stringStream >> word;

        if (std::stringstream(word) >> code) {
            return code;
        }
        
        word = "";
    }
    return -1;
} 

// se extrage cookie ul de login
void extractCookies(std::vector<std::string>& cookies, std::string response) {

    std::string responseCookie;
    int start = response.find("Set-Cookie: ");
    int endOfLine = response.find("\n", start);
    responseCookie = response.substr(start + 12, endOfLine - (start + 13));
    cookies.push_back(responseCookie);

}

// extragere token din raspunsul enter_library
std::string extractToken(std::string response) {
    
    int startJSON = response.find("{");
    int endJSON = response.find("}", startJSON);
    json tokenJSON;
    tokenJSON = json::parse(response.substr(startJSON, endJSON - startJSON + 1));

    return tokenJSON["token"];    

}

bool validFormat(std::string title, std::string author, std::string genre,
                 std::string pageCount, std::string publisher) {

    if (title == "" || author == "" || genre == "" || pageCount == ""
        || publisher == "") {
            return false;
    }

    if (!isNumeric(pageCount)) {
        return false;
    }

    return true;
}

// construire lista de book-jsons
void extractBooks(std::string response, std::vector<json>& books) {

    // se cauta lista de jsons
    int start = response.find('[');
    int final = response.find(']');

    if (final - start == 1) {
        return;
    }
    std::string jsonList = response.substr(start + 1, final - start - 1);
    // se parseaza fiecare json
    do {
        start = jsonList.find('{');
        final = jsonList.find('}');
        books.push_back(json::parse(jsonList.substr(start, final - start + 1)));
        jsonList = jsonList.erase(0, final + 2);
    } while(jsonList != "");
   
} 

// se construieste un json de tip book 
json extractBook(std::string response) {
    
    int start = response.find('{');
    int final = response.find('}');

    return json::parse(response.substr(start, final - start + 1));
}
// se verifica daca un string este numeric
bool isNumeric(std::string numeric) {
    
    for (size_t i = 0; i < numeric.size(); i++) {
        if (numeric[i] < '0' || numeric[i] > '9') {
            return false;
        }
    }
    
    return true;
}