#ifdef __cplusplus

#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <iostream>
//#include <format>
#include <cstring>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <chrono>

#define BUFSIZE 8192
#define INVALID_SOCKET -1

// template<typename... Args>
// void errorhandling(const std::string_view fmt_str, Args&&... args){
//     auto fmt_args{ std::make_format_args(args...) };
//     std::string outstr{ std::vformat(fmt_str, fmt_args) };
//     fputs(outstr.c_str(), stderr);
//     //if(system("read"));
// };

// template<typename... Args>
// void print(const std::string_view fmt_str, Args&&... args){
//     auto fmt_args{ std::make_format_args(args...) };
//     std::string outstr{ std::vformat(fmt_str, fmt_args) };
//     fputs(outstr.c_str(), stdout);
// };

// void showtime(){
// 	// 获取当前系统时间点
//     auto currentTime = std::chrono::system_clock::now();
//     // 将时间点转换为时间结构
//     std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
//     std::tm localTime = *std::localtime(&currentTime_t);
//     // 使用 chrono::duration 表示微秒
//     // auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(
//     //     currentTime.time_since_epoch() % std::chrono::seconds(1)
//     // );
//     // 打印本地时间的各个组成部分
//     std::cout << "Current local time: "
//               << localTime.tm_year + 1900 << "-"  // 年份，需要加上 1900
//               << localTime.tm_mon + 1 << "-"       // 月份，需要加上 1
//               << localTime.tm_mday << " "          // 日
//               << localTime.tm_hour << ":"          // 时
//               << localTime.tm_min << ":"           // 分
//               << localTime.tm_sec << ".\n\n"  ;         // 秒
//               //<< microseconds.count() << std::endl; // 微秒
// }


// void gettimebystring(std::string &str){
// 	auto currentTime = std::chrono::system_clock::now();
// 	std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
//     std::tm localTime = *std::localtime(&currentTime_t);
// 	str += std::format("Current local time: {}-{}-{} {}:{}:{}\n",
// 			localTime.tm_year + 1900,
// 			localTime.tm_mon + 1,
// 			localTime.tm_mday,
// 			localTime.tm_hour,
// 			localTime.tm_min,
// 			localTime.tm_sec);
// }

#endif



#endif