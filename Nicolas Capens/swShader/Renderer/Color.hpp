#ifndef Color_hpp
#define Color_hpp

#include "Types.hpp"

namespace swShader
{
	template<class T>
	struct Color
	{
		Color();
		Color(const Color &c);
		Color(unsigned int c);
		Color(T r, T g, T b, T a = 1);

		operator unsigned int() const;

		Color operator+() const;
		Color operator-() const;

		Color& operator=(const Color& c);

		Color &operator+=(const Color &c);
		Color &operator*=(float l);

		static Color gradient(const Color &c1, const Color  &c2, float d);
		static Color shade(const Color &c1, const Color  &c2, float d);
		static Color average(const Color &c1, const Color &c2);

		friend Color operator+(const Color &c1, const Color &c2);
		friend Color operator-(const Color &c1, const Color &c2);

		friend Color operator*(float l, const Color &c);
		friend Color operator*(const Color &c1, const Color &c2);
		friend Color operator/(const Color &c, float l);

		static void setGamma(float gamma);

		T b;
		T g;
		T r;
		T a;

		static const Color<float> WHITE;
		static const Color<float> BLACK;
		static const Color<float> GRAY;
		static const Color<float> BLUE;
		static const Color<float> CYAN;
		static const Color<float> GREEN;
		static const Color<float> YELLOW;
		static const Color<float> RED;
		static const Color<float> MAGENTA;

	private:
		static float gamma;

		static byte averageLUT[256][256];
		static byte gammaLUT[256];
	};
}

#include "Math.hpp"

namespace swShader
{
	template<class T>
	inline Color<T>::Color()
	{
	}

	template<class T>
	inline Color<T>::Color(const Color &c)
	{
		r = c.r;
		g = c.g;
		b = c.b;
		a = c.a;
	}

	inline Color<float>::Color(unsigned int c)
	{
		const float d = 1.0f / 255.0f;

		r = (float)((c & 0x00FF0000) >> 16) * d;
		g = (float)((c & 0x0000FF00) >> 8) * d;
		b = (float)((c & 0x000000FF) >> 0) * d;
		a = (float)((c & 0xFF000000) >> 24) * d;
	}

	inline Color<byte>::Color(unsigned int c)
	{
		*(unsigned int*)this = c;
	}

	template<class T>
	inline Color<T>::Color(T r_, T g_, T b_, T a_)
	{
		r = r_;
		g = g_;
		b = b_;
		a = a_;
	}

	inline Color<float>::operator unsigned int() const
	{
		return ((unsigned int)min(b * 255.0f, 255.0f) << 0) |
		       ((unsigned int)min(g * 255.0f, 255.0f) << 8) |
		       ((unsigned int)min(r * 255.0f, 255.0f) << 16) |
		       ((unsigned int)min(a * 255.0f, 255.0f) << 24);
	}

	inline Color<byte>::operator unsigned int() const
	{
		return *(unsigned int*)this;
	}

	template<class T>
	inline Color<T> Color<T>::operator+() const
	{
		return *this;
	}

	template<class T>
	inline Color<T> Color<T>::operator-() const
	{
		return Color(-r, -g, -b, -a);
	}

	template<class T>
	inline Color<T> &Color<T>::operator=(const Color& c)
	{
		r = c.r;
		g = c.g;
		b = c.b;
		a = c.a;

		return *this;
	}

	template<class T>
	inline Color<T> &Color<T>::operator+=(const Color &c)
	{
		r += c.r;
		g += c.g;
		b += c.b;
		a += c.a;

		return *this;
	}

	template<class T>
	inline Color<T> &Color<T>::operator*=(float l)
	{
		*this = l * *this;

		return *this;
	}

	template<class T>
	inline Color<T> operator+(const Color<T> &c1, const Color<T> &c2)
	{
		return Color<T>(c1.r + c2.r,
		                c1.g + c2.g,
		                c1.b + c2.b,
		                c1.a + c2.a);	
	}

	template<class T>
	inline Color<T> operator-(const Color<T> &c1, const Color<T> &c2)
	{
		return Color<T>(c1.r - c2.r,
		                c1.g - c2.g,
		                c1.b - c2.b,
		                c1.a - c2.a);	
	}

	template<class T>
	inline Color<T> operator*(float l, const Color<T> &c)
	{
		T r = (T)(l * c.r);
		T g = (T)(l * c.g);
		T b = (T)(l * c.b);
		T a = (T)(l * c.a);

		return Color<T>(r, g, b, a);
	}

	template<class T>
	inline Color<T> operator*(const Color<T> &c1, const Color<T> &c2)
	{
		T r = c1.r * c2.r;
		T g = c1.g * c2.g;
		T b = c1.b * c2.b;
		T a = c1.a * c2.a;

		return Color<T>(r, g, b, a);
	}

	inline Color<byte> operator*(const Color<byte> &c1, const Color<byte> &c2)
	{
		byte r = c1.r * c2.r >> 8;
		byte g = c1.g * c2.g >> 8;
		byte b = c1.b * c2.b >> 8;
		byte a = c1.a * c2.a >> 8;

		return Color<byte>(r, g, b, a);
	}

	template<class T>
	inline Color<T> operator/(const Color<T> &c, float l)
	{
		l = 1.0f / l; 

		T r = (T)(l * c.r);
		T g = (T)(l * c.g);
		T b = (T)(l * c.b);
		T a = (T)(l * c.a);

		return Color<T>(r, g, b, a);
	}

	template<class T>
	inline Color<T> Color<T>::gradient(const Color<T> &c1, const Color<T> &c2, float d)
	{
		d = 1.0f / d; 

		T r = (c2.r - c1.r) * d;
		T g = (c2.g - c1.g) * d;
		T b = (c2.b - c1.b) * d;
		T a = (c2.a - c1.a) * d;

		return Color<T>(r, g, b, a);
	}

	template<class T>
	inline Color<T> Color<T>::shade(const Color<T> &c1, const Color<T>  &c2, float d)
	{
		T r = c1.r + (T)(d * (c2.r - c1.r));
		T g = c1.g + (T)(d * (c2.g - c1.g));
		T b = c1.b + (T)(d * (c2.b - c1.b));
		T a = c1.a + (T)(d * (c2.a - c1.a));

		return Color<T>(r, g, b, a);
	}

	inline Color<float> Color<float>::average(const Color<float> &c1, const Color<float>  &c2)
	{
		return Color<float>(pow((pow(c2.r, gamma) + pow(c1.r, gamma)) / 2, 1 / gamma),
		                    pow((pow(c2.g, gamma) + pow(c1.g, gamma)) / 2, 1 / gamma),
		                    pow((pow(c2.b, gamma) + pow(c1.b, gamma)) / 2, 1 / gamma),
		                    pow((pow(c2.a, gamma) + pow(c1.a, gamma)) / 2, 1 / gamma));
	}

	inline Color<byte> Color<byte>::average(const Color<byte> &c1, const Color<byte>  &c2)
	{
		return Color<byte>(averageLUT[c1.r][c2.r],
		                   averageLUT[c1.g][c2.g],
		                   averageLUT[c1.b][c2.b],
		                   averageLUT[c1.a][c2.a]);
	}

	template<class T>
	void Color<T>::setGamma(float gamma)
	{
		if(Color<T>::gamma == gamma) return;

		Color<T>::gamma = gamma;

		for(int i = 0; i < 256; i++)
		{
			gammaLUT[i] = (T)(255 * pow((float)i / 255, 1 / gamma));

			for(int j = 0; j < 256; j++)
			{
				averageLUT[i][j] = (T)(255 * pow((pow((float)i / 255, gamma) + pow((float)j / 255, gamma)) / 2, 1 / gamma));
			}
		}
	}
}

#endif   // Color_hpp