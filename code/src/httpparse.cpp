#include "httpparse.hpp"

#include <chrono>
#include <iostream>
#include <sstream>

namespace vastina
{

    HttpParser::HttpParser() : method{INVALID}, version{NOT_SUPPORTED}, path{}, body{}, headers{} {};

    HttpParser::~HttpParser()
    {
        body.clear();
        headers.clear();
    };

    void HttpParser::Reset()
    {
        headers.clear();
        method = INVALID;
        version = NOT_SUPPORTED;
        path.clear();
        body.clear();
    }

    METHOD HttpParser::getMethod() const { return method; }
    const std::string& HttpParser::getPath() const { return path; }
    VERSION HttpParser::getProtocol() const { return version; }
    const std::string& HttpParser::getBody() const { return body; }

    void HttpParser::ParseMethod(const std::string& tmp)
    {
        switch (tmp[0])
        {
        case 'G':
            if (tmp[1] == 'E' && tmp[2] == 'T')
                method = GET;
            break;
        case 'P':
            switch (tmp[1])
            {
            case 'O':
                if (tmp[2] == 'S' && tmp[3] == 'T')
                    method = POST;
                break;
            case 'U':
                if (tmp[2] == 'T')
                    method = PUT;
                break;
            case 'A':
                if (tmp[2] == 'T' && tmp[3] == 'C' && tmp[4] == 'H')
                    method = PATCH;
                break;
            default: break;
            }
            break;
        case 'D':
            if (tmp[1] == 'E' && tmp[2] == 'L' && tmp[3] == 'E' && tmp[4] == 'T' && tmp[5] == 'E')
                method = DELETE;
            break;
        case 'H':
            if (tmp[1] == 'E' && tmp[2] == 'A' && tmp[3] == 'D')
                method = HEAD;
            break;
        case 'T':
            if (tmp[1] == 'R' && tmp[2] == 'A' && tmp[3] == 'C' && tmp[4] == 'E')
                method = TRACE;
            break;
        default: break;
        }
    }

    void HttpParser::ParseVersion(const std::string& tmp)
    {
        if (tmp == "HTTP/1.1")
            version = HTTP_11;
        else if (tmp == "HTTP/1.0")
            version = HTTP_10;
        else if (tmp == "HTTP/0.9")
            version = HTTP_09;
    }

    void HttpParser::autoparse(const char* buf)
    {
        auto t1 = std::chrono::high_resolution_clock::now();

        std::istringstream buf_stream(buf);
        parts part = START_LINE;
        std::string line;
        while (getline(buf_stream, line))
        {
            switch (part)
            {
            case START_LINE: {
                std::istringstream line_stream(line);
                std::string tmp;
                line_stream >> tmp;
                if (tmp.find("HTTP") == std::string::npos)
                {
                    ParseMethod(tmp);
                    line_stream >> path;
                    line_stream >> tmp;
                    ParseVersion(tmp);
                }
                part = HEADERS;
                break;
            }
            case HEADERS: {
                if (line.size() == 1)
                {
                    part = BODY;
                    break;
                }
                auto pos = line.find(":");
                if (pos == std::string::npos)
                    continue;
                std::string tmp1(line, 0, pos);
                std::string tmp2(line, pos + 2);
                headers.insert(std::make_pair(format_key(tmp1), tmp2));
                break;
            }
            case BODY: {
                body.append(line);
                body.push_back('\n');
                break;
            }
            default: break;
            }
        }

        auto t2 = std::chrono::high_resolution_clock::now();
        std::cout << "parse time: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << '\n';
    }

    void HttpParser::autoparse(const std::string& buf) { autoparse(buf.c_str()); }

    std::string HttpParser::operator[](const std::string& str)
    {
        auto it = headers.find(str);
        return it != headers.end() ? it->second : "";
    }

    std::string HttpParser::format_key(std::string& str)
    {
        if (str[0] >= 'A' && str[0] <= 'Z')
        {
            str[0] = str[0] - 'A' + 'a';
        }
        unsigned long position = 0;
        while ((position = str.find("-", position)) != std::string::npos)
        {
            if (str[position + 1] >= 'A' && str[position + 1] <= 'Z')
            {
                str[position + 1] = str[position + 1] - 'A' + 'a';
            }
            position++;
        }
        return str;
    }

} // namespace vastina