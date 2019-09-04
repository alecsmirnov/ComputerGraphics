#ifndef CLOSEDBSPLINE_H
#define CLOSEDBSPLINE_H

#include <cstdint>
#include <vector>

namespace ClosedBSpline {
	struct Point {
		int x;
		int y;
	};

	std::vector<Point> calculate(const std::vector<Point>& points, std::uint8_t degree, double step);
}

#endif