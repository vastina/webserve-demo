#ifndef _HTTP_PARSE_H_
#define _HTTP_PARSE_H_

#include <string>
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <regex>

namespace vastina{
    
// constexpr enum METHOD{
//      GET = 0
//     ,POST
//     ,DELETE
//     ,PUT
//     ,HEAD
//     ,TRACE
//     ,CONNECT
//     ,OPTIONS
//     ,PATCH
// };


class httpparser{

private:
    //int state;
    std::map<std::string, std::string> results;
    std::string format_key(std::string &str);
public:
    httpparser(): results{} {};
    ~httpparser(){};
    void autoparse(const char* buf);
    void autoparse(std::string& buf);
    void reset();

    std::string operator[](std::string str);
    std::string operator[](std::string& str);

    std::string getMethod();
    std::string getPath();
    std::string getProtocol();
    std::string getBody();
};

void httpparser::reset(){
    results.clear();
}

std::string httpparser::getMethod() { return results["method"]; }
std::string httpparser::getPath() { return results["path"]; }
std::string httpparser::getProtocol() { return results["version"]; }
std::string httpparser::getBody() { return results["body"]; }

void httpparser::autoparse(const char* buf){
    std::istringstream buf_stream(buf);
    enum parts{
        start_line,
        headers,
        body
    };
    parts part = start_line;
    std::string line;
    std::string body_string;
    while(getline(buf_stream, line)){
        switch (part)
        {
            case start_line:
            {
                std::istringstream line_stream(line);
                std::string tmp;
                line_stream >> tmp;
                if(tmp.find("HTTP") == std::string::npos){
                    results.insert(std::make_pair("method", tmp));
                    line_stream >> tmp;
                    results.insert(std::make_pair("path", tmp));
                    line_stream >> tmp;
                    results.insert(std::make_pair("version", tmp));
                } else{
                    results.insert(std::make_pair("version", tmp));
                    line_stream >> tmp;
                    results.insert(std::make_pair("status", tmp));
                    line_stream >> tmp;
                    results.insert(std::make_pair("status_text", tmp));
                }
                part = headers;
                break;
            }
            case headers:
            {
                if(line.size() == 1){
                    part = body;
                    break;
                }
                auto pos = line.find(":");
                if(pos == std::string::npos)
                    continue;
                std::string tmp1(line, 0, pos);
                std::string tmp2(line, pos + 2);
                results.insert(std::make_pair(format_key(tmp1), tmp2));
                break;
            }
            case body:
            {
                body_string.append(line);
                body_string.push_back('\n');
                break;
            }
            default:
                break;
        }
    }
    results.insert(std::make_pair("body", body_string));
}

void httpparser::autoparse(std::string& buf){
    autoparse(buf.c_str());
}

std::string httpparser::operator[](std::string str){
    auto it = results.find(format_key(str));
    return it != results.end() ? it->second : "";
}

std::string httpparser::operator[](std::string& str){
    auto it = results.find(format_key(str));
    return it != results.end() ? it->second : "";
}

std::string httpparser::format_key(std::string &str){
    if(str[0] >= 'A' && str[0] <= 'Z'){
        str[0] = str[0] - 'A' + 'a';
    }
    unsigned long position = 0;
    while((position = str.find("-", position)) != std::string::npos){
        if(str[position + 1] >= 'A' && str[position + 1] <= 'Z'){
            str[position + 1] = str[position + 1] - 'A' + 'a';
        }
        position++;
    }
    return str;
}

}



#endif