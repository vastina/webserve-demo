#ifndef _HTTP_RESPONSE_H_
#define _HTTP_RESPONSE_H_

#include <filesystem>
#include <string>
#include <string_view>
#include <set>
#include <fstream>
#include <cstring>

#include "httpparse.hpp"

namespace vastina{

enum STATUS_CODE{
     CONTINUE = 100
    ,SWITCHING_PROTOCOLS                //101 switching_protocols
    ,PROCESSING
    ,OK = 200                           //200 ok
    ,CREATED
    ,ACCEPTED
    ,NON_AUTHORITATIVE_INFORMATION
    ,NO_CONTENT
    ,RESET_CONTENT
    ,PARTIAL_CONTENT
    ,MULTI_STATUS
    ,ALREADY_REPORTED
    ,IM_USED
    ,MULTIPLE_CHOICES = 300
    ,MOVED_PERMANENTLY
    ,FOUND
    ,SEE_OTHER
    ,NOT_MODIFIED
    ,USE_PROXY
    ,TEMPORARY_REDIRECT
    ,BAD_REQUEST = 400
    ,UNAUTHORIZED
    ,PAYMENT_REQUIRED
    ,FORBIDDEN
    ,NOT_FOUND                          //404 not_fonud
    ,METHOD_NOT_ALLOWED
    ,NOT_ACCEPTABLE
    ,PROXY_AUTHENTICATION_REQUIRED
    ,REQUEST_TIMEOUT
    ,CONFLICT
    ,GONE
    ,LENGTH_REQUIRED
    ,PRECONDITION_FAILED
    ,REQUEST_ENTITY_TOO_LARGE
    ,REQUEST_URI_TOO_LONG
    ,UNSUPPORTED_MEDIA_TYPE
    ,REQUESTED_RANGE_NOT_SATISFIABLE
    ,EXPECTATION_FAILED
    ,INTERNAL_SERVER_ERROR = 500
    ,NOT_IMPLEMENTED
    ,BAD_GATEWAY
    ,SERVICE_UNAVAILABLE 
    ,GATEWAY_TIMEOUT 
    ,HTTP_VERSION_NOT_SUPPORTED         //505 http_version_not_supported
};


namespace fs = std::filesystem;

class cachetree{//to do
private:
    cachetree(): static_files{ } {}

    std::set<std::string> static_files;
public:
    static cachetree& getInstance(){
        static cachetree cache;
        return cache;
    }
    void init_read(const fs::path& directory = fs::current_path()
        , const fs::path& relativePath = fs::path());

    bool static_file_exist(std::string& str);
    bool static_file_exist(std::string str);
};

bool cachetree::static_file_exist(std::string& str) {
    return static_files.find(str) != static_files.end();
}

bool cachetree::static_file_exist(std::string str) {
    return static_files.find(str) != static_files.end();
}

void cachetree::init_read(const fs::path& directory, const fs::path& relativePath) {
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_directory()) {
            fs::path newRelativePath = relativePath / entry.path().filename();
            init_read(entry.path(), newRelativePath);  // 递归遍历子目录
        } else if (entry.is_regular_file()) {
            static_files.insert((relativePath / entry.path().filename()).string()); 
        }
    }
}

class httpresponse
{
private:
    ;
public:
    httpresponse(){};
    ~httpresponse(){};
    void init();
    void makereponse_test(httpparser& parser, char *buf);//no buffer now, so send parser directly
};

void justfortest(std::string *response){
    response->append("HTTP/1.1 ").append(std::to_string(STATUS_CODE::OK)).append(" OK\r\n");
    response->append("Date: Thu, 20 Jan 2022 12:00:00 GMT\r\n");
    response->append("Server: MyCustomServer/1.0\r\n");
    response->append("Content-Type: text/html charset=utf-8\r\n");
    response->append("Connection: keep-alive\r\n");
    response->append("\r\n");
}

void httpresponse::makereponse_test(httpparser& parser, char *buf){
    std::string *response = new std::string("");
        response->reserve(200);
        justfortest(response);
        strcpy(buf, response->c_str());
        int count = response->size();
std::cout << "count: "<<count <<'\n';
    delete response;

    if(parser.getMethod()=="GET" && parser.getPath()=="/" && parser.getProtocol()=="HTTP/1.1"){
        if(cachetree::getInstance().static_file_exist("index.html")){
std::cout << "cache\n";
            std::ifstream infile;
            infile.open("index.html", std::ifstream::in);
            infile.read(buf+count, BUFSIZ-count);
            infile.close();
        }
    }
}


}





#endif // _HTTP_RESPONSE_H_