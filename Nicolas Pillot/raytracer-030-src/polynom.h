#ifndef POLYNOM_H
#define POLYNOM_H

#include <vector>
#include <set>
#include "printable.h"

class Polynom :	public Printable
{
	private:
		typedef std::vector<double> Coefficients;
		Coefficients coeffs;

		void setLengthFill(int length);
		double scalarProd(const Polynom& poly) const;

	public: 
		Polynom(double zeroordervalue = 0);
		Polynom(double a, double b);
		Polynom(double a, double b, double c);
		Polynom(double a, double b, double c, double d);
		// Polynom(const std::string& initstring);
		virtual ~Polynom();
		// Epsilon is used to round near zero values to zero
		static void setEpsilon(double _epsilon);

		// Accessors
		void setCoeff(int index, double value);
		double getCoeff(int index) const;
		double& operator [] (int index);

		int getOrder() const;

		// polynom operations
		Polynom operator + (const Polynom& poly) const;
		Polynom operator - (const Polynom& poly) const;
		Polynom operator * (const Polynom& poly) const;
		Polynom operator * (double lambda) const;
		Polynom operator / (double lambda) const;
		void operator += (const Polynom& poly);
		void operator -= (const Polynom& poly);
		void operator *= (const Polynom& poly);
		void operator *= (double lambda);
		void operator /= (double lambda);
		Polynom insertPoly(const Polynom& poly) const;
		Polynom derivate() const;
		Polynom integrate() const;
		Polynom pow(int degree) const;
		double	evaluate(double t) const;

		virtual std::string toString() const;

		typedef std::set<double> Zeros;
		Zeros getZeros() const;
	private:
		void solve1(Zeros& zeros) const;
		void solve2(Zeros& zeros) const;
		void solve3(Zeros& zeros) const;
};

#endif

