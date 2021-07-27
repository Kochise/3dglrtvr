#ifndef __FILEPROCS_HPP_
#define __FILEPROCS_HPP_

#include <iostream.h>
#include <fstream.h>

char *IntToStr(int i);
bool isWhiteSpace(char ch);

class FileDecoder {
  private:
    ifstream fin;
    char fname[256];
  public:
    FileDecoder(const char *filename);
    ~FileDecoder();

    void  SkipSpaces();
    bool  eof();
    float ReadFloat();
    int   ReadInt();
    char  PeekChar();
    char  ReadChar();
    bool  PeekString(char *str);
    bool  ReadString(char *str);
    bool  FileDecoder::ReadTextString(char *res);
    bool  ReadTagString(char *tagname, char *res);  // Read data between xml tags
    int   ReadTagInt(char *tagname);                // Read data between xml tags
    float ReadTagFloat(char *tagname);              // Read data between xml tags
    void  LogFileError(char *fmt, ...);
};

#endif /* __FILEPROCS_HPP_ */

