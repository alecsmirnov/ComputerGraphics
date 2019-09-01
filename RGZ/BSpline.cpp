#include "BSpline.h"

namespace BSpline {

static constexpr double PARTITION_MIN = 0.0;
static constexpr double PARTITION_MAX = 1.0;

static double divCheck(double x, double y) {
	return y == 0 ? 0 : x / y;
}

static double blend(const std::vector<double>& knot, double x, std::vector<Point>::size_type i, std::vector<Point>::size_type degree) {
	double result = 0;

	if (degree == 0)
		result = knot[i] <= x && x < knot[i + 1] ? 1 : 0;
	else
		result = divCheck(x - knot[i], knot[i + degree] - knot[i]) * blend(knot, x, i, degree - 1) +
				 divCheck(knot[i + degree + 1] - x, knot[i + degree + 1] - knot[i + 1]) * blend(knot, x, i + 1, degree - 1);

	return result;
}

static std::vector<double> linSpace(double start, double stop, std::vector<Point>::size_type control_points_count) {
	std::vector<double> line;

	if (0 < control_points_count)
		if (control_points_count == 1)
			line.push_back(start);
		else
			for (std::vector<Point>::size_type i = 0; i != control_points_count; ++i)
				line.push_back(start + i * (stop - start) / (control_points_count - 1));

	return line;
}

static std::vector<double> getClosedKnot(std::vector<Point>::size_type points_count, std::vector<Point>::size_type degree) {
	double sector = points_count - degree;

	return linSpace(-static_cast<double>(degree) / sector, static_cast<double>(points_count) / sector, points_count + degree + 1);
}

std::vector<Point> calculate(const std::vector<Point>& points, std::vector<Point>::size_type degree, double step) {
	std::vector<Point> spline_points;

	if (degree < points.size()) {
		auto control_points = points;
		for (std::vector<Point>::size_type i = 0; i != degree; ++i)
			control_points.push_back(control_points[i]);

		auto knot = getClosedKnot(control_points.size(), degree);

		for (double partition = PARTITION_MIN; partition < PARTITION_MAX; partition += step) {
			Point spline_point = {0, 0};

			for (std::vector<Point>::size_type i = 0; i != control_points.size(); ++i) {
				auto spline = blend(knot, partition, i, degree);

				spline_point.x += static_cast<int>(control_points[i].x * spline);
				spline_point.y += static_cast<int>(control_points[i].y * spline);
			}

			spline_points.push_back(spline_point);
		}
	}

	return spline_points;
}

}