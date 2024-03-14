#include "httpresponse.hpp"

#include <fstream>
#include <iostream>

namespace vastina
{

    bool CacheTree::StaticFileExist(const std::string& str) { return static_files.find(str) != static_files.end(); }

    void CacheTree::InitRead(const fs::path& directory, const fs::path& relativePath)
    {
        const std::string directoryPath = "./";
        fs::path absDirectoryPath = fs::absolute(directoryPath);

        for (const auto& entry : fs::recursive_directory_iterator(absDirectoryPath))
        {
            fs::path relativePath = fs::relative(entry.path(), absDirectoryPath);
            static_files.insert(relativePath.string());
        }

        // test
        for (const auto& file : static_files) { std::cout << file << std::endl; }
    }

    void writefile(char* buf, int count, std::string filename)
    {
        std::ifstream infile;
        infile.open(filename, std::ifstream::in);
        infile.read(buf + count, BUFSIZ - count);
        infile.close();
    }

    HttpResponse::HttpResponse()
        : filename{"index.html"}, state{OK}, connection{KEEP_ALIVE}, content_type{TEXT}, length{default_body_length} {};

    HttpResponse::~HttpResponse()
    {
        filename.clear();
        body.clear();
    };

    void HttpResponse::SolveRequestLine(HttpParser& parser)
    {
        switch (parser.getProtocol())
        {
        case HTTP_11: {
            switch (parser.getMethod())
            {
            case GET: SolvePath(parser.getPath()); break;
            case POST:
                /* code */
                break;
            default: break;
            }
            break;
        }
        default: {
            filename = "505.html";
            state = HTTP_VERSION_NOT_SUPPORTED;
            content_type = TEXT;
            connection = CLOSE;
            break;
        }
        }
    }

    void HttpResponse::SolvePath(const std::string& str)
    {
        if (str == "/")
        {
            filename = "index.html";
            state = OK;
            content_type = TEXT;

            length = default_body_length;
        }
        else if (str == "/test")
        {
            filename = "204.html";
            state = ACCEPTED;
            content_type = TEXT;

            length = default_body_length;
        }
        else if (str == "/login?text=test")
        {
            // todo: parse action?body
            filename = "page2/gettest.html";
            state = OK;
            content_type = TEXT;

            length = default_body_length;
        }
        else
        {
            filename = str.substr(1);
            if (!CacheTree::getInstance().StaticFileExist(filename))
            {
                filename = "404.html";
                state = NOT_FOUND;
                content_type = TEXT;

                length = default_body_length;
            }
            else
                state = OK;
            if (state == OK)
            {
                if (filename.find(".html") != std::string::npos)
                    content_type = TEXT;
                else if (filename.find(".ico") != std::string::npos)
                    content_type = ICON;
                else if (filename.find(".gif") != std::string::npos)
                    content_type = GIF;
                else if (filename.find(".png") != std::string::npos)
                    content_type = PNG;
                else if (filename.find(".jpeg") != std::string::npos)
                    content_type = JPEG;
                else if (filename.find(".audio") != std::string::npos)
                    content_type = AUDIO;
                else if (filename.find(".video") != std::string::npos)
                    content_type = VIDEO;
                else if (filename.find(".json") != std::string::npos)
                    content_type = JSON;
                else if (filename.find(".js") != std::string::npos)
                    content_type = SCRIPT;
                else
                    content_type = OTHER;
            }
        }
    }

    void HttpResponse::AddHeader(std::string& response)
    {
        response.append("HTTP/1.1 ").append(STATUS_STR.at(state));
        response.append("Date: Thu, 20 Jan 2022 12:00:00 GMT\r\n");
        response.append("Server: localhost:6780\r\n");
        response.append("Content-Length: ").append(std::to_string(length)).append("\r\n");
        response.append("Content-Type: ").append(CONTENNT_TYPE_STR.at(content_type)).append(" charset=utf-8\r\n");
        response.append(CONNECTION_STR.at(connection));
        response.append("\r\n");
    }

    void HttpResponse::autoResponse(HttpParser& parser, char* buf)
    {
        std::string response;
        response.reserve(default_header_length);
        memset(buf, 0, BUFSIZ);

        if (parser["connection"] == "close\r")
        {
            connection = CLOSE;
        }
        else
        {
            connection = KEEP_ALIVE;
        }

        SolveRequestLine(parser);

        AddHeader(response);

        int len = response.size();
        strcpy(buf, std::move(response.c_str()));

        writefile(buf, len, filename);

        // delete response;
    }

    void HttpResponse::Reset()
    {
        filename.clear();
        state = NOT_FOUND;
        connection = NO_CONNECTION;
        content_type = TEXT;
        length = default_body_length;
    }

} // namespace vastina