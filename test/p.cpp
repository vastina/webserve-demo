// parser test

#include <fstream>
#include "../http/HttpParser.h"

char buffer[2048];

int main(){
    std::fstream fs;
        fs.open("request.txt", std::ifstream::in | std::ios::binary);
        fs.read(buffer, BUFSIZ);
    fs.close();

    HttpParser parser = HttpParser(buffer);
    parser.show();

    std::cout << parser["Method"] <<'\n';
    std::cout << parser["Path"] <<'\n';
    std::cout << parser["Version"] <<'\n';
}