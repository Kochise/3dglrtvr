/** Color.h
 */

#ifndef COLOR_H
#define COLOR_H

class Color
{
public:
	Color();
	Color(double vRed, double vGreen, double vBlue);
	~Color();
	
	Color operator* (double vRhs) const; // returns multiplication of a scalar with a vector
	Color operator+ (const Color& vRhs) const; // returns the addition of this color with another color

	void Red(double vRed);
	double Red() const;
	void Green(double vGreen);
	double Green() const;
	void Blue(double vBlue);
	double Blue() const;

	unsigned char RedByte() const;
	unsigned char GreenByte() const;
	unsigned char BlueByte() const;
private:
	unsigned char ByteValue(double vZeroToOne) const;
	double mRed, mGreen, mBlue;
};

#endif
