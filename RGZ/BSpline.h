#ifndef BSPLINE_H
#define BSPLINE_H

#include <vector>

namespace BSpline {
	struct Point {
		int x;
		int y;
	};

	std::vector<Point> calculate(const std::vector<Point>& points, std::vector<Point>::size_type degree, double step);
}

#endif