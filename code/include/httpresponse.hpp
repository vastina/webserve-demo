#ifndef _HTTP_RESPONSE_H_
#define _HTTP_RESPONSE_H_

#include <cstring>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>

#include "config.hpp"
#include "httpparse.hpp"

namespace vastina {

namespace fs = std::filesystem;

class cachetree
{ // to do
private:
  cachetree() : file_content {} {}

  // std::set<std::string> static_files;
  struct fileview
  {
    char* text { nullptr };
    u64 length {};

    void readfile( const std::string& filename )
    {
      length = fs::file_size( filename );
      text = new char[length];
      std::ifstream infile;
      infile.open( filename, std::ifstream::in );
      infile.readsome( text, length );
      infile.close();
    }
  };
  std::unordered_map<std::string, fileview> file_content;

public:
  static cachetree& getInstance()
  {
    static cachetree cache;
    return cache;
  }
  ~cachetree()
  {
    for ( auto& [key, value] : file_content ) {
      delete[] value.text;
    }
  }
  void init_read( const fs::path& directory = fs::current_path() );

  bool static_file_exist( const std::string& str );
  inline fileview& getfilecontent( const std::string& str )
  {
    auto& view { file_content[str] };
    if ( view.length <= 0 || nullptr == view.text )
      view.readfile( str );
    return view;
  }
};

constexpr size_t default_header_length = 200;
constexpr size_t default_body_length = 2000;
// 随便写的
class httpresponse
{

private:
  std::string filename;
  std::string body;
  STATUS_CODE state;
  CONNECTION connection;
  CONTENT_TYPE content_type;

public:
  httpresponse();
  ~httpresponse();

  void reset();
  void makeresponse( const httpparser& parser, int fd );
  void solveRequest( const httpparser& parser );
  void solvepath( const std::string& str );
  void addheader( int );
  std::string addheader ( ) const;
};

} // namespace vastina

#endif // _HTTP_RESPONSE_H_