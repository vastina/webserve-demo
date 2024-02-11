#ifndef _LOGER_H_
#define _LOGER_H_

#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>

namespace vastina {

void logtest(const char *buffer);
void logtime(std::fstream &fp);

}; // namespace vastina

#endif