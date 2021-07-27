#include <iostream.h>
#include <fstream.h>
#include <stdarg.h>
#include <stdio.h>

#include "FileProcs.hpp"
#include "Log.hpp"

/****************************/

char *IntToStr(int i)
{
  static char res[256];
  sprintf(res, "%i", i);
  return res;
}

// A whitespace can be one of following;
//   blank space       ' '
//   form-feed         '\f'
//   newline           '\n'
//   carriage return   '\r'
//   horizontal tab    '\t'
//   vertical tab      '\v'
bool isWhiteSpace(char ch)
{
  switch(ch) {
    case ' ': return true; break;
    case '\f': return true; break;
    case '\n': return true; break;
    case '\r': return true; break;
    case '\t': return true; break;
    case '\v': return true; break;
    default: return false;
  }
}

/****************************/

FileDecoder::FileDecoder(const char *filename) {
  strcpy(fname,filename);
  fin.open(fname, ios::binary | ios::in);
  if (fin.good() == false) Log_Exit("Can't open file: '%s'", fname);
  LOG("Open file: '%s'", fname);
}

FileDecoder::~FileDecoder() {
  fin.close();
  LOG("Close file: '%s'", fname);
}

void FileDecoder::SkipSpaces() {
  char buf[256];
  if (fin.good())
    if ((fin.peek()=='#') || (isWhiteSpace(fin.peek())))
      for(; (fin.good()) && ((fin.peek()=='#') || (isWhiteSpace(fin.peek())));)
        if (fin.get()=='#') { fin.getline(buf, 256); }   // Uses # as "skip rest of line" for remarks
};

bool FileDecoder::eof() {
  SkipSpaces();
  return fin.eof();
};

float FileDecoder::ReadFloat() {
  float res,div,sign;
  if (eof()) return 0;
  res = 0;
  div = 0;
  if (fin.peek()=='-') { fin.get(); sign = -1; } else { sign = 1; }
  while ((fin.peek()=='.') || ((fin.peek()>='1') && (fin.peek()<='9')) || (fin.peek()=='0')) {
  	if (fin.peek()=='.') {
  		div = 1;
  		fin.get();
 		} else {
 			div = div*10;
	  	res = res*10 + 1 + (fin.get()-'1');
  	}
 	}
 	if (div>0) {
 		res = res / div;
 	}
 	res *= sign;
  //fin.scan("%f", &res);
  LOG("File '%s' read float %f", fname, res);
  return res;
};

int FileDecoder::ReadInt() {
  int res,sign;
  if (eof()) return 0;
  res = 0;
  if (fin.peek()=='-') { fin.get(); sign = -1; } else { sign = 1; }
  while (((fin.peek()>='1') && (fin.peek()<='9')) || (fin.peek()=='0')) {
  	res = res*10 + 1 + (fin.get()-'1');
 	}
 	res *= sign;
  //fin.scan("%i", &res);
  LOG("File '%s' read int %i", fname, res);
  return res;
};

char FileDecoder::PeekChar() {
  if (eof()) return 0;
  return fin.peek();
};

char FileDecoder::ReadChar() {
  if (eof()) return 0;
  return fin.get();
};

bool FileDecoder::PeekString(char *str) {
  int i;
  streampos pos;
  bool res = false;
  if (eof()) return false;
  if (fin.good())
  {
    pos = fin.tellg();
    for(i=0; (i<strlen(str)) && (fin.good()) && (fin.get()==str[i++]);) ;
    if (i==(strlen(str)))
      res = true;
    fin.seekg(pos);
  }
  LOG("File '%s' peek string '%s' (%s)", fname, str, (res)?"Found":"Not found");
  return res;
};

bool FileDecoder::ReadString(char *str) {
  int i;
  streampos pos;
  bool res = false;
  if (eof()) return false;
  if (fin.good())
  {
    pos = fin.tellg();
    for(i=0; (i<strlen(str)) && (fin.good()) && (fin.get()==str[i++]);) ;
    if (i==(strlen(str)))
      res = true;
    else
      fin.seekg(pos);
  }
  LOG("File '%s' read string '%s' (%s)", fname, str, (res)?"Found":"Not found");
  return res;
};

// Reads the string between delimiters (double pings) "..."
bool FileDecoder::ReadTextString(char *res) {
  int i;
  bool result;
  char ch;
  res[0] = 0;
  result = false;
  ch = PeekChar();
  if (ch=='\"')
  {
    ch = ReadChar();
    for(i=0; PeekChar()!='\"' && (i<250); res[i++]=fin.get());
    res[i++] = 0;
    ch = ReadChar();
    result = true;
  }
//  LOG("File '%s' read text string \"%s\"", fname, res);
  return result;
};

// Reads the string between tags <name>...</name>
bool FileDecoder::ReadTagString(char *tagname, char *res) {
  int i;
  char str[64];
  bool result;
  res[0] = 0;
  result = false;
  sprintf(str, "<%s>", tagname);
  if (ReadString(str))
  {
    sprintf(str, "</%s>", tagname);
    for(i=0; !PeekString(str); res[i++]=fin.get());
    res[i++] = 0;
    result = true;
    if (!ReadString(str)) LogFileError("Missing end tag '%s'", tagname);
  } else LogFileError("Missing start tag '%s'", tagname);
//  LOG("File '%s' read tag <%s>%s</%s>", fname, tagname, result, tagname);
  return result;
};

// Reads the int between tags <name>...</name>
int FileDecoder::ReadTagInt(char *tagname) {
  int result=0;
  char str[64];

  sprintf(str, "<%s>", tagname);
  if (ReadString(str))
  {
    result = ReadInt();
    sprintf(str, "</%s>", tagname);
    if (!ReadString(str)) LogFileError("Missing end tag '%s'", str);
  } else LogFileError("Missing start tag '%s'", str);
  return result;
};

// Reads the float between tags <name>...</name>
float FileDecoder::ReadTagFloat(char *tagname) {
  float result=0.0;
  char str[64];

  sprintf(str, "<%s>", tagname);
  if (ReadString(str))
  {
    result = ReadFloat();
    sprintf(str, "</%s>", tagname);
    if (!ReadString(str)) LogFileError("Missing end tag '%s'", str);
  } else LogFileError("Missing start tag '%s'", str);
  return result;
};

void FileDecoder::LogFileError(char *fmt, ...) {
  streampos pos, lpos;
  va_list args;
  int line, p, lp;
  char buf[256];
  va_start(args, fmt);
  vsprintf(buf, fmt, args);
  va_end(args);
  Log("File error %s", buf);
  if (fin.good() && !fin.eof())
  {
    pos = fin.tellg();
    fin.seekg(ios::beg);
    lpos = fin.tellg();
    for (line=0; (((int)(pos))>=((int)(fin.tellg()))) && (!fin.eof()); lpos=fin.tellg(), fin.getline(buf, 254), line++);
    fin.seekg(pos);
  }
  Log("File error line %i in file '%s'", line, fname);
  Log(">> %s", buf);
  for (p=0; (p<((int)(pos)-(int)(lpos))) && (p<250); p++) buf[p]=' ';
  buf[p++] = '^';
  buf[p++] = 0;
  Log_Exit(">> %s", buf);
}

