// parser test

#include <fstream>
#include "../http/HttpParser.h"

char buffer[2048];

int main(){
    std::fstream fs;
        fs.open("request.txt", std::ifstream::in | std::ios::binary);
        fs.read(buffer, BUFSIZ);
    fs.close();

    HttpParser *parser = new HttpParser(buffer);
    parser->show();
    delete parser;
}