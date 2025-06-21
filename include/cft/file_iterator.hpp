#pragma once
#include <fstream>
#include <string>

class FileIterator {
 private:
  int currentLine = 0;
  std::fstream file;
  std::string path;
  std::string line_buf;

 public:
  FileIterator (const std::string& path) : file (path, std::ios::in),  path {path} {}

  FileIterator& operator<<(const auto& write) {
    file << write;
    return *this;
  }

  inline bool fetchNext(std::string& output) {
     bool tryline = static_cast<bool> (std::getline(file, output));
    ++currentLine;
    return tryline;
  }

  inline int forEachLine(auto action, FileIterator* Log) {
    while (fetchNext(line_buf)) {
      if (Log)
        Log->file << action(line_buf);
      ++currentLine;
    }
    return currentLine;
  }

  inline std::string getPath() {return path;}
  inline int getCurrentLine() {return currentLine;}
};

