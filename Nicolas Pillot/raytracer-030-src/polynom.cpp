
#include <cmath>
#include "polynom.h"
#include "exceptions.h"

Polynom::Polynom(double zeroordervalue)
{
	coeffs.push_back(zeroordervalue);
}

Polynom::Polynom(double a, double b)
{
	coeffs.push_back(b);
	coeffs.push_back(a);
}

Polynom::Polynom(double a, double b, double c)
{
	coeffs.push_back(c);
	coeffs.push_back(b);
	coeffs.push_back(a);
}

Polynom::Polynom(double a, double b, double c, double d)
{
	coeffs.push_back(d);
	coeffs.push_back(c);
	coeffs.push_back(b);
	coeffs.push_back(a);
}

Polynom::~Polynom()
{
}

int Polynom::getOrder() const
{
	int order = int(coeffs.size()) - 1;
	Coefficients::const_reverse_iterator it = coeffs.rbegin();
	for (; (it != coeffs.rend()) && (*it == 0.0); it++)
		order --;
	return order;
}

double Polynom::scalarProd(const Polynom& poly) const
{
	double tmp = 0;
	int maxorder = getOrder()+1;
	if (maxorder < poly.getOrder()+1)
		maxorder = poly.getOrder()+1;
	for (int i = 0; i < maxorder; i++)
		tmp += poly.getCoeff(i) * getCoeff(i);
	return tmp;
}

std::string Polynom::toString() const
{
    std::string tmp("Polynom { ");

	int order = getOrder();
	for (Coefficients::const_reverse_iterator it = coeffs.rbegin(); it != coeffs.rend(); it++)
	{
		if ((*it != 0) || (order == 0))
		{
			if (*it >= 0)
				tmp += '+';
			tmp += Utils::value2string(*it);
			tmp += 'x';
			tmp += '^';
			tmp += Utils::value2string(order);
		}
		order --;
	}
	tmp += " }";
	return tmp;
}

void Polynom::setLengthFill(int length)
{
	if (length >= int(coeffs.size()))
		for (int num = length - int(coeffs.size()) + 1; num > 0; num--)
			coeffs.push_back(0.0);
}

void Polynom::setCoeff(int index, double value) 
{
	setLengthFill(index);
	if (index >= int(coeffs.size()))
		throw InvalidParameterException("Polynom : index out of bound");
	coeffs[index] = value;
}

double Polynom::getCoeff(int index) const
{
	if ((index >= int(coeffs.size())) || (index < 0))
		return 0.0;
	else
		return coeffs[index];
}

double& Polynom::operator [] (int index)
{
	setLengthFill(index);
	if (index >= int(coeffs.size()))
		throw InvalidParameterException("Polynom : index out of bound");
	return coeffs[index];
}

Polynom Polynom::operator + (const Polynom& poly) const
{
	Polynom p(poly);
	int maxorder = getOrder();
	if (maxorder < p.getOrder())
		maxorder = p.getOrder();
	for (int i = 0; i <= maxorder; i++)
		p[i] += getCoeff(i);
	return p;
}

Polynom Polynom::operator - (const Polynom& poly) const
{
	Polynom p(*this);
	int maxorder = getOrder();
	if (maxorder < p.getOrder())
		maxorder = p.getOrder();
	for (int i = 0; i <= maxorder; i++)
		p[i] -= poly.getCoeff(i);
	return p;
}

Polynom Polynom::operator * (double lambda) const
{
	Polynom p(*this);
	for (int i = p.getOrder() + 1; i >= 0; i--)
		p[i] *= lambda;
	return p;
}

Polynom Polynom::operator / (double lambda) const
{
	if (lambda == 0)
		throw MathException("Polynom : divide by zero");
	Polynom p(*this);
	for (int i = p.getOrder() + 1; i >= 0; i--)
		p[i] /= lambda;
	return p;
}

Polynom Polynom::operator * (const Polynom& poly) const
{
	Polynom p;
	int order_sum = poly.getOrder() + getOrder() + 1;
	for (int i = 0; i < order_sum; i++)
		for (int j = 0; j <= i; j++)
			p[i] += poly.getCoeff(i-j) * getCoeff(j);
	return p;
}

Polynom Polynom::pow(int degree) const
{
	if (degree < 0)
		throw InvalidParameterException("Polynom::pow : invalid degree ( < 0)");

	Polynom p (1);
	for (int i = 0; i < degree; i++)
		p = p * (*this);
	return p;
}

void Polynom::operator *= (const Polynom& poly)
{
	*this = *this * poly;
}

void Polynom::operator *= (double lambda)
{
	*this = *this * lambda;
}

void Polynom::operator /= (double lambda)
{
	*this = *this / lambda;
}

void Polynom::operator += (const Polynom& poly)
{
	*this = *this + poly;
}

void Polynom::operator -= (const Polynom& poly)
{
	*this = *this - poly;
}

Polynom Polynom::insertPoly(const Polynom& poly) const
{
	Polynom p;
	for (int i = getOrder(); i >= 0; i--)
		p += (poly.pow(i) * getCoeff(i));
	return p;
}

Polynom Polynom::derivate() const
{
	Polynom p;
	for (int i = getOrder() - 1; i >= 0; i--)
		p[i] = getCoeff(i+1) * (i+1);
	return p;
}

Polynom Polynom::integrate() const
{
	// Warning, does set the constant term to zero !
	Polynom p;
	for (int i = getOrder() + 1; i >= 1; i--)
		p[i] = getCoeff(i-1) / i;
	return p;
}

double Polynom::evaluate(double t) const
{
	double total = 0;
	for (int i = getOrder(); i >= 0; i--)
	{
		double temp = 1;
		for (int j = 0; j < i; j++)
			temp *= t;
		total += temp * getCoeff(i);
	}
	return total;
}


Polynom::Zeros Polynom::getZeros() const
{
	Zeros zeros;
	switch(getOrder())
	{
	case 1:
		solve1(zeros);
		break;
	case 2:
		solve2(zeros);
		break;
	case 3:
		solve3(zeros);
		break;
	}
	return zeros;
}

void Polynom::solve1(Zeros& zeros) const
{
	zeros.insert(-getCoeff(0) / getCoeff(1));
}

void Polynom::solve2(Zeros& zeros) const
{
	double delta = getCoeff(1) * getCoeff(1) - 4 * getCoeff(2) * getCoeff(0);
	if (delta > 0)
	{
		zeros.insert((- getCoeff(1) - sqrt(delta)) / (2 * getCoeff(2)));
		zeros.insert((- getCoeff(1) + sqrt(delta)) / (2 * getCoeff(2)));
	}
	else
		if (delta == 0.0)
			zeros.insert(- getCoeff(1) / (2 * getCoeff(2)));
}

void Polynom::solve3(Zeros& zeros) const
{
	// normalize polygon to p[3] = 1
	Polynom p (*this / getCoeff(3));
	
	double q = (p[2] * p[2] - 3 * p[1]) / 9.0;
	double r = (p[2] * ( p[2] * p[2] - 4.5 * p[1] ) + 13.5 * p[0]) / 27.0;
	double delta = q * q * q - r * r;

	// 3 real roots
	if (delta >= 0.0) 
	{
		// q is always different from zero here
		double tdelta = r / (sqrt(q) * sqrt(q) * sqrt(q));
		double tau = acos( tdelta ) / 3.0;
		double s = -2.0 * sqrt(q);
		// computes the three solution
		zeros.insert(s * cos(tau) - p[2] / 3.0);
		zeros.insert(s * cos(tau+2.094395) - p[2] / 3.0);
		zeros.insert(s * cos(tau+4.18879) - p[2] / 3.0);
	}
	// 1 real root (delta < 0)
	else
	{
		double s = sqrt(-delta) + fabs(r);
		// s = s^(1/3), because a^b = exp(b*ln(a))
		s = exp( log(s) / 3.0 );
		double x;
		// s is always positive, non zero here
		if (r < 0)
			x = s + q / s - p[2] / 3.0;
		else
			x = - s - q / s - p[2] / 3.0;
		zeros.insert(x);
	}
}

