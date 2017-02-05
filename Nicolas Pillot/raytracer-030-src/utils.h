#ifndef UTILS_H
#define UTILS_H

#include <sstream>
#include <string>

#include "logconsole.h"

class Utils
{
	public:
		static LogConsole log;
		static const double PI, PI_2;
		static double epsilon;
		void setEpsilon(double _epsilon);

		template <class T>
		static std::string value2string(const T& val)
		{
			std::ostringstream os;
			os << val;
			return os.str();
		}

		template <class T>
		static std::string value2string(const std::string& name, const T& val)
		{
			std::ostringstream os;
			os << name;
			os << " = ";
			os << val;
			return os.str();
		}

		template <class T>
		static void swap(T& elem1, T& elem2)
		{
			T temp = elem1;
			elem1 = elem2;
			elem2 = temp;
		}

		static double deg2rad(const double& deg);
		static double rad2deg(const double& rad);
};

#endif

