#ifndef _TOOL_H_
#define _TOOL_H_

// #ifndef __cplusplus

// #include <stdio.h>
// #include <stdbool.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include <sys/socket.h>
// #include <errno.h>

// void errorhandling(const char* message, int error ){
// 	printf("%s : %d \n", message, error);
// };

// bool quitjudge(const char* s){
//     if(s[0]=='q' && strlen(s)==1) return true;
//     if(s[0]=='q' && s[1]=='u' && s[2]=='i' && s[3]=='t' && strlen(s)==4) return true;
//     return false;
// }; 

// bool sendfile(const char* s){
// 	if(s[0]=='f' && s[1]=='i' && s[2]=='l' && s[3]=='e' && strlen(s)==4) return true;
// 	return false;
// }


// int CreateSeverSocket(int af,int type,int protocol)
// {
// 	//创建空SOCKET
// 	//第一个参数 地址协议ipv4 ipv6 
// 	//第二个参数 传输协议类型 流式套接字 数据包 
// 	//第三个参数 使用具体的某个传输协议
// 	//传输协议为 TCP
// 	int so = socket(af, type, protocol);
// 	int opt = 0, len = sizeof(opt);
// 	// if( setsockopt(so, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, len))
// 	// 	errorhandling("invalid socket: ", errno );
// 	//
// 	//TCP连接失败
// 	if (so == -1)
// 	{
//         errorhandling("invalid socket: ", errno );
// 		return -1;
// 	}
// 	//给socket绑定ip地址和端口号
// 	struct sockaddr_in addr;
//     memset(&addr, 0, sizeof(addr));
// 	addr.sin_family = af;//和创建socket时必须一样
// 	short PORT = 8888;
// 	addr.sin_port = htons(PORT);//define htons 本地字节转网络字节
// 	addr.sin_addr.s_addr = inet_addr("127.0.0.1");//网卡的任意ip//inet_addr("127.0.0.1");//转为二进制
// 	if (bind(so, (struct sockaddr*)&addr, sizeof(addr)) == -1)
// 	{
//         errorhandling("connect fail with code: ", errno );
// 		return false;
// 	}
// 	//监听
// 	listen(so, 5);
// 	return so;
// }

// int CreateClientSocket(int af,int type,int protocol){
//     int so = socket(af, type, protocol);
//     if (so == -1)
// 	{
//         errorhandling("invalid socket: ", errno);
// 		return -1;
// 	}
//     struct sockaddr_in serveaddr;
//     memset(&serveaddr, 0, sizeof(serveaddr));
//     serveaddr.sin_family = af;
//     serveaddr.sin_addr.s_addr= inet_addr("127.0.0.1");
// 	short PORT = 8888; 
// 	//printf("input port:");	scanf("%hd", &PORT);
//     serveaddr.sin_port = htons(PORT);
//     if (connect(so, (struct sockaddr*)&serveaddr, sizeof(serveaddr)) == -1)
//         errorhandling("connect fail: ", errno);
    
//     return so;
	
// }
// #endif

#ifdef __cplusplus

#include <iostream>
#include <format>
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