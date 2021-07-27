#ifndef IMAGE_H
#define IMAGE_H

#include <cstdio>
#include "color.h"

class Image : public Printable
{
    private:
        int imagewidth;
        int imageheight;
		int scanlinelength;
        FILE * handle;
		
    public:
        Image(const std::string& _name, int _width, int _height);
        Image(const std::istream& is);
        virtual ~Image();

        void setPixelAt(int x, int y, const Color& color);
        
        int width() const;
        int height() const;

        virtual std::string toString() const;
        static Image instanciateFromStream(const std::istream& is);
        
    private:
        void writeByte(unsigned char c);
        void writeWord(unsigned short int i);
        void writeDword(unsigned long int j);        
};

#endif

