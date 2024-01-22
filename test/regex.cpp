#include <iostream>
#include <regex>

int main() {
    // HTTP 请求报文的第一行
    const char* request_line = "GET /vastina/index.html HTTP/1.1";

    // 定义正则表达式模式，捕获请求方法、路径和协议版本
    std::regex request_regex(R"((\w+) (\S+) (\S+))");

    // 使用 std::cmatch 对象存储匹配结果
    std::cmatch match;

    // 尝试匹配正则表达式
    if (std::regex_match(request_line, match, request_regex)) {
        // match[0] 匹配整个字符串
        // match[1] 匹配请求方法
        // match[2] 匹配路径
        // match[3] 匹配协议版本
        std::cout << "Method: " << match[1] << std::endl;
        std::cout << "Path: " << match[2] << std::endl;
        std::cout << "Protocol: " << match[3] << std::endl;
    } else {
        std::cout << "Invalid HTTP request line." << std::endl;
    }

    return 0;
}
// #include <iostream>
// #include <regex>

// int main() {
//     // 原始字符串
//     const char* input = "This is a line of text.\r\n114514345";

//     // 定义正则表达式模式，匹配\r\n前面的内容
//     std::regex regex_pattern(".*?(\r\n|$)");

//     // 使用 std::smatch 对象存储匹配结果
//     std::cmatch match;

//     // 尝试匹配正则表达式
//     if (std::regex_match(input, match, regex_pattern)) {
//         // match[0] 匹配整个字符串
//         // match[1] 匹配\r\n前面的内容
//         std::cout << "Content before \\r\\n: " << match[1] << std::endl
//         << match[2] <<'\n';
//     } else {
//         std::cout << "No match found." << std::endl;
//     }

//     return 0;
// }
// #include <iostream>
// #include <regex>

// int main() {
//     // 原始字符串
//     std::string input = "This is a line of text.\r\nRemaining content after \\r\\n.";

//     // 定义正则表达式模式，匹配\r\n后面的内容
//     std::regex regex_pattern(".*?\\r\\n(.*)");

//     // 使用 std::smatch 对象存储匹配结果
//     std::smatch match;

//     // 尝试匹配正则表达式
//     if (std::regex_match(input, match, regex_pattern)) {
//         // match[0] 匹配整个字符串
//         // match[1] 匹配\r\n后面的内容
//         std::cout << "Content after \\r\\n: " << match[1] << std::endl;
//     } else {
//         std::cout << "No match found." << std::endl;
//     }

//     return 0;
// }
// #include <iostream>
// #include <regex>

// int main() {
//     // 原始字符串
//     const char *input = "GET /vastina/index.html HTTP/1.1\r\nRemaining \r\ncontent after \\r\\n.";

//     // 定义正则表达式模式，匹配\r\n前面的内容
//     std::regex regex_pattern("(.*)\\r\\n.*");

//     // 使用 std::smatch 对象存储匹配结果
//     std::cmatch match;

//     // 尝试匹配正则表达式
//     if (std::regex_match(input, match, regex_pattern)) {
//         // match[0] 匹配整个字符串
//         // match[1] 匹配\r\n前面的内容
//         std::cout << "Content before \\r\\n: " << match[1] << std::endl;

//         std::string re(match[1]);
//         std::regex request_regex(R"((\w+) (\S+) (\S+))");
//         std::smatch lmatch;

//         if (std::regex_match(re, lmatch, request_regex)) {
//             std::cout << "Method: " << lmatch[1] << std::endl;
//             std::cout << "Path: " << lmatch[2] << std::endl;
//             std::cout << "Protocol: " << lmatch[3] << std::endl;
//         }
//     } 
//     return 0;
// }
// #include <iostream>
// #include <regex>
// #include <fstream>
// #include <string.h>

// int main() {
//     char input[BUFSIZ];
//     memset(input, 0, BUFSIZ);
//     std::ifstream infile;
//     infile.open("request.txt", std::ifstream::in);
//     while (!infile.eof())
//     {
//         infile.read(input, BUFSIZ);
//     }
//     infile.close();

//     // 定义正则表达式模式，匹配第一个\r\n前面的内容
//     std::regex regex_pattern("([^\r\n]+)");

//     // 使用 std::smatch 对象存储匹配结果
//     std::cmatch match;

//     // 尝试匹配正则表达式
//     if (std::regex_search(input, match, regex_pattern)) {
//         // match[0] 匹配整个字符串
//         // match[1] 匹配第一个\r\n前面的内容
//         std::cout << "Content before first \\r\\n: " << match[1] << std::endl;
//         std::string re(match[1]);
//         std::regex request_regex(R"((\w+) (\S+) (\S+))");
//         std::smatch lmatch;
//         if (std::regex_match(re, lmatch, request_regex)) {
//             std::cout << "Method: " << lmatch[1] << std::endl;
//             std::cout << "Path: " << lmatch[2] << std::endl;
//             std::cout << "Protocol: " << lmatch[3] << std::endl;
//         }
//     } else {
//         std::cout << "No match found." << std::endl;
//     }

//     return 0;
// }
