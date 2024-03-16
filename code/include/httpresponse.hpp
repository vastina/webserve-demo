#ifndef _HTTP_RESPONSE_H_
#define _HTTP_RESPONSE_H_

#include <cstring>
#include <filesystem>
#include <set>
#include <string>

#include "httpparse.hpp"

namespace vastina
{

    namespace fs = std::filesystem;

    class CacheTree
    { // to do
      private:
        CacheTree() : static_files{} {}
        ~CacheTree();

        std::set<std::string> static_files;

      public:
        static CacheTree& getInstance()
        {
            static CacheTree cache;
            return cache;
        }
        void InitRead(const fs::path& directory = fs::current_path(), const fs::path& relativePath = fs::path());

        bool StaticFileExist(const std::string& str);
    };

    constexpr size_t default_header_length = 200;
    constexpr size_t default_body_length = 2000;
    // 随便写的
    class HttpResponse
    {

      private:
        std::string filename;
        std::string body;
        STATUS_CODE state;
        CONNECTION connection;
        CONTENT_TYPE content_type;
        size_t length;

      public:
        HttpResponse();
        ~HttpResponse();

        void Reset();

        void autoResponse(HttpParser& parser, char* buf);
        void SolveRequestLine(HttpParser& parser);
        void SolvePath(const std::string& str);
        void AddHeader(std::string& response);
    };

} // namespace vastina

#endif // _HTTP_RESPONSE_H_