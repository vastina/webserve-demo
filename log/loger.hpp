#ifndef _LOGER_H_
#define _LOGER_H_

#include <fstream>

namespace vastina {

void logtest( const char* buffer );
void logtime( std::fstream& fp );

}; // namespace vastina

#endif