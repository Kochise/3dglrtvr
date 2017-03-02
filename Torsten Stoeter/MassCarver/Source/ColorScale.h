#ifndef COLOR_SCALE_H
#define COLOR_SCALE_H

class ColorScale
{
	private:
		double (*scale)[5];
		int n, c;

	public:
		ColorScale(int num);
		~ColorScale();
		void AddPoint(double v, double r, double g, double b, double a);
		void GetColor(double v, double& r, double& g, double& b, double& a);
};

#endif

