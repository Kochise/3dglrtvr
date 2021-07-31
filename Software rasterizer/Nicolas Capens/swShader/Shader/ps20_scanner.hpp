/******************************************************************
 * File: ps20_scanner.hpp
 *
 * This file was automatically generated by CppCC Version 0.0.5
 * DO NOT EDIT MANUALLY
 ******************************************************************/


#ifndef __PS20_SCANNER_HH__
#define __PS20_SCANNER_HH__

#include <string>
#include <fstream>
#include <iostream>
#include <exception>
#include <deque>
#include <stack>
using namespace std;

#include "ps20_token.hpp"

#pragma warning(disable : 4290)
#pragma warning(disable : 4786)


namespace swShader
{

class ScanException : public exception
{
  public:
    
    ScanException (const string &message_ = "Scan exception") :
      message(message_)
    {}
    
    ScanException (const Position &pos_,
                   const string &message_ = "Scan exception") :
      message(message_), pos(pos_)
    {}
    
    ~ScanException () throw ()
    {}
    
    virtual operator string () const
    {
      return string(pos) + ": " + message;
    }
    
    virtual const char* what () throw ()
    {
      return message.c_str();
    }
    
  private:
  
    Position pos;
    string message;
};

class ps20_scanner_base
{
  protected:
    bool onScanError (ScanException &ex) { return false; }
    bool onIOError (ScanException &ex) { return false; }
    bool wrap () { return false; }
    void commonTokenAction (ps20_token &tok) {}
    void onInit () {}
    void onDestroy () {}
};

class ps20_scanner: public ps20_scanner_base
{
public:
  
  struct t_laCell
  {
    ps20_token *token;
    struct t_laCell *next;
    t_laCell(ps20_token *token_, struct t_laCell *next_) : token(token_), next(next_) {}
    ~t_laCell() { delete token; }
  };
  
  ps20_scanner (istream *in_ = NULL) throw (ScanException)
      : _buffer(NULL), _reject(false), _more(false),
  _bsz(8 * 1024), _pbsz(100)
  {
    switchToStream(in_);
    lookahead = false;
    la0 = _laBegin = _laEnd = new struct t_laCell(new ps20_token, NULL);
    la0->next = la0;
    switchToState(START);
    onInit();
  }
  
  ~ps20_scanner () { onDestroy(); }
  
  void rejectToken() {_reject = true; }
  
  void switchToStream (istream *in)
  {
    resetPos();
    inputStream = in;
    _binit();
  }
  
  class StreamState
  {
    friend class ps20_scanner;
    istream *inputStream;
    char *_buffer, *_ch, *_start, *_eob;
    int _bsz, _pbsz;
    bool _lta, _eof;
    Position bPos, ePos;
    
    StreamState (istream *inputStream_, char *_buffer_, int _bsz_,
                int _pbsz_, char *_start_, char *_ch_,
                 char *_eob_, bool _lta_, bool _eof_,
                 Position &bPos_, Position &ePos_) :
      inputStream(inputStream_), _buffer(_buffer_), _bsz(_bsz_),
       _pbsz(_pbsz_), _start(_start_), _ch(_ch_),
       _eob(_eob_), _lta(_lta_), _eof(_eof_),
       bPos(bPos_), ePos(ePos_)
    {}
    
    ~StreamState () { if (_buffer != NULL) delete[] _buffer; }
  };
  
  StreamState* pushStream (istream *in)
  {
    StreamState *res = new StreamState(inputStream, _buffer, _bsz,
                              _pbsz, _more ? _start : _ch, _ch,
                                       _eob, _lta, _eof,
                                     _more ? bPos : ePos, ePos);
    _buffer = NULL;
    switchToStream(in);
    return res;
  }
  
  void popStream (StreamState *s)
  {
    if (_buffer != NULL) delete[] _buffer;
    inputStream = s->inputStream;
    _buffer = s->_buffer;
    _bsz = s->_bsz;
    _pbsz = s->_pbsz;
    _start = s->_start;
    _ch = s->_ch;
    _eob = s->_eob;
    _lta = s->_lta;
    _eof = s->_eof;
    bPos = s->bPos;
    ePos = s->ePos;
    s->_buffer = NULL;
    delete s;
  }
  
  istream& getInputStream ()
  {
    return *inputStream;
  }
  
  int switchToState (int newState)
  {
    int prevState = _state;
    _state = newState;
    return prevState;
  }
  
  int getState ()
  {
    return _state;
  }
  
  int pushState (int newState)
  {
    _stateStack.push(_state);
    return switchToState(newState);
  }
  
  int popState ()
  {
    int prevState = switchToState(_stateStack.top());
    _stateStack.pop();
    return prevState;
  }
  
  const Position& getCurrentPos ()
  {
    return bPos;
  }
  
  void resetPos ()
  {
    ePos.ln = bPos.ln = 1;
  }
  
  int newLine ()
  {
    return ePos.ln++;
  }
  
  bool lookingAhead ()
  {
    return lookahead;
  }
  
  int getChar () throw (ScanException)
  {
    for(;;) {
      unsigned int c = *_ch++;
      if (_ch == _eob) {
        _ch--;
        if (_eof && !wrap()) return EOF;
        else _brefill();
      } else {
        if (_start == _ch - 1) _start = _ch;
        return c;
      }
    }
  }
  
  void unGetChars (const char *c, int n);
  void unGetChar (char c)
  {
    unGetChars(&c, 1);
  }
  
  void unGetChars (const string &s)
  {
    unGetChars(s.data(), s.length());
  }
  
  void unGetChars (const char *s)
  {
    unGetChars(s, strlen(s));
  }
  
  ps20_token* la (int k) throw (ScanException)
  {
    struct t_laCell *p, *q;
    for (p = la0; k >= 0; k--, q = p, p = p->next) {
      if (p == _laEnd) {
        if (_laEnd->next == _laBegin)
          _laEnd->next = new struct t_laCell(new ps20_token(), _laBegin);
        _scan(_laEnd->token);
        _laEnd = _laEnd->next;
      }
    }
    return q->token;
  }
  
  ps20_token* la () throw (ScanException)
  {
    if (la0 == _laEnd) {
      if (_laEnd->next == _laBegin)
        _laEnd->next = new struct t_laCell(new ps20_token(), _laBegin);
      _scan(_laEnd->token);
      _laEnd = _laEnd->next;
    }
    return la0->token;
  }
  
  void consume () throw (ScanException)
  {
    la();
    la0 = la0->next;
    if (!lookahead)
      _laBegin = _laBegin->next;
  }
  
  void unchecked_consume ()
  {
    la0 = la0->next;
    if (!lookahead)
      _laBegin = _laBegin->next;
  }
  
  void setMarker ()
  {
    lookahead = true;
    laMarkers.push(la0);
  }
  
  void rewindToMarker ()
  {
    la0 = laMarkers.top();
    laMarkers.pop();
    if (laMarkers.empty()) {
      lookahead = false;
      _laBegin = la0;
    }
  }
  
  
  static const int START;
  static const int LONG_COMMENT;
  
protected: // the ugly stuff

  void _scan(ps20_token *token) throw (ScanException);
  Position bPos, ePos;
  istream *inputStream;
  
  bool lookahead;
  struct t_laCell *la0, *_laBegin, *_laEnd;
  stack<struct t_laCell*> laMarkers;
  int _state;
  stack<int> _stateStack;
  bool _reject;
  
  void _brefill () throw (ScanException);
  void _binit() throw (ScanException);
  int _bsz; //buffer size
  int _pbsz; //extra bytes for pushback
  char *_buffer;
  char *_ch; // current char
  char *_eob; // one past the last char in the buffer
  char *_start; // points ot the token's start
  bool _eof; // eof seen during _brefill
  bool _lta; // last token in the stream was accepted
  bool _more; // true in token actions of MORE tokens
  
}; // end class ps20_scanner


} //end namespace swShader

#endif // ifndef __PS20_SCANNER_HH__


/* End of file. */