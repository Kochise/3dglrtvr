
#include "image.h"
#include "exceptions.h"

int Image::width() const
{
    return imagewidth;
}
    
int Image::height() const
{
    return imageheight;
}

void Image::writeByte(unsigned char c)
{
    fputc(c , handle);
}

void Image::writeWord(unsigned short int i)
{
    fwrite( & i, sizeof(unsigned short int), 1 , handle);
}

void Image::writeDword(unsigned long int j)
{
    fwrite( & j, sizeof(unsigned long int), 1 , handle);
}

Image::Image(const std::string& _name, int _width, int _height)
: imagewidth(_width), imageheight(_height)
{
    if (imagewidth < 1)
        throw InvalidParameterException("Invalid image width (must be positive)");
    if (imageheight < 1)
        throw InvalidParameterException("Invalid image height (must be positive)");
    
    handle = fopen(_name.c_str(), "wb");
    if (handle == NULL)
        throw FileException("BmpFile : Unable to create file \"" + std::string (_name) + "\"");

	int dummybytes = (4 - (imagewidth * 3) % 4) % 4;
	scanlinelength = (imagewidth * 3) + dummybytes;

    // please see for BMP file format
    // http://www.daubnet.com/formats/BMP.html
    writeByte('B');
    writeByte('M');
    writeDword(14 + 40 + imageheight * scanlinelength);
    writeDword(0);
    writeDword(14 + 40);
    writeDword(40);
    writeDword(imagewidth);
    writeDword(imageheight);
    writeWord(1);
    writeWord(24);
    writeDword(0);
    writeDword(0);
    writeDword(0);
    writeDword(0);
    writeDword(0);
    writeDword(0);
    
    for (int i = 0; i < imageheight * scanlinelength; i++)
        writeByte(0);
}

Image::~Image()
{
    if (handle != NULL)
    {
        fclose(handle);
        handle = NULL;
        imagewidth = 0;
        imageheight = 0;
    }
}

void Image::setPixelAt(int x, int y, const Color& color)
{
    // clipping
    if ((x < 0) || (y < 0) || (x >= imagewidth) || (y >= imageheight))
        throw InvalidParameterException("Image : Pixel index (" + Utils::value2string(x) + ',' + Utils::value2string(y) + ") out of image limits.");

    // vertical inversion
    y = (imageheight - 1) - y;
    // set position into file
    fseek(handle, (14 + 40) + (y * scanlinelength + x * 3), SEEK_SET);
    // computes data to write
    unsigned char red   = (unsigned char) ((float)color.getRed()  * 255.0),
                  green = (unsigned char) ((float)color.getGreen()* 255.0),
                  blue  = (unsigned char) ((float)color.getBlue() * 255.0);
    
    // writes data to file
    writeByte(blue);
    writeByte(green);
    writeByte(red);
}

std::string Image::toString() const
{
    return "Image { "
        + Utils::value2string("Width", imagewidth) + ' '
		+ Utils::value2string("Height", imageheight) + " }";
}

Image Image::instanciateFromStream(const std::istream& is)
{
	return Image("name",10,10);
}


