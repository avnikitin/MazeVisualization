#pragma once
#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>

namespace geometry {

	class Point;
	class Line;
	class Segment;
	class Circle;
	class Rectangle;

	const double eps = 1e-7;

	bool eq(const double a, const double b) {
		return (a < b + eps && b < a + eps);
	}

	bool less(const double a, const double b) {
		return (a < b && !eq(a, b));
	}

	bool leq(const double a, const double b) {
		return eq(a, b) || less(a, b);
	}

	bool greater(const double a, const double b) {
		return (a > b && !eq(a, b));
	}

	bool geq(const double a, const double b) {
		return greater(a, b) || eq(a, b);
	}

	class Point {
		double x, y;

	public:
		Point() : x(0), y(0) {}
		Point(double _x, double _y): x(_x), y(_y) {}

		Point operator + (const Point other) const {
			return { x + other.x, y + other.y };
		}

		void operator += (const Point other) {
			x += other.x;
			y += other.y;
		}

		Point operator - (const Point other) const {
			return { x - other.x, y - other.y };
		}

		void operator -= (const Point other) {
			x -= other.x;
			y -= other.y;
		}
		
		bool operator == (const Point other) const {
			return eq(x, other.x) && eq(y, other.y);
		}

		bool operator != (const Point other) const {
			return !(*this == other);
		}

		bool operator <= (const Point other) const {
			return leq(x, other.x) && leq(y, other.y);
		}

		bool operator < (const Point other) const {
			return less(x, other.x) && less(y, other.y);
		}

		bool operator >= (const Point other) const {
			return !(*this < other);
		}

		bool operator > (const Point other) const {
			return !(*this <= other);
		}

		double get_x() const {
			return x;
		}

		double get_y() const {
			return y;
		}
		
		friend double distance(const Point, const Point);
		friend double vector_product(const Point, const Point);
		friend double scalar_product(const Point, const Point);
		friend Point max(const Point, const Point);
		friend Point min(const Point, const Point);

		friend class Line;
		friend double distance(const Point, const Line);
		friend double distance(const Line, const Point);
		friend bool doesPointBelongToALine(const Point, const Line);
		friend bool doLinesIntersect(const Line, const Line);

		friend class Circle;
		friend bool doCirclesIntersect(const Circle, const Circle);
		friend bool doSegmentAndCircleIntersect(const Segment, const Circle);

		friend class Segment;
		friend bool doesPointBelongToASegment(const Point, const Segment);
		friend bool doSegmentsIntersect(const Segment, const Segment);
		friend double distance(const Point, const Segment);
		friend double distance(const Segment, const Point);
		friend double distance(const Segment, const Segment);

		friend class Rectangle;
		friend bool doRectanglesIntersect(const Rectangle, const Rectangle);
		friend bool doesPointBelongToARectangle(const Point, const Rectangle);
		friend std::vector<Segment> BordersOfRectangle(const Rectangle);
		friend bool doRectangleAndCircleIntersect(const Rectangle, const Circle);
		friend bool doSegmentAndRectangleIntersect(const Segment, const Rectangle);
	};

	double vector_product(const Point a, const Point b) {
		return a.x * b.y - a.y * b.x;
	}

	double scalar_product(const Point a, const Point b) {
		return a.x * b.x + a.y * b.y;
	}
	
	double distance(const Point a, const Point b) {
		return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
	}

	Point max(const Point a, const Point b) {
		return { std::max(a.x, b.x), std::max(a.y, b.y) };
	}

	Point min(const Point a, const Point b) {
		return { std::min(a.x, b.x), std::min(a.y, b.y) };
	}

	class Line {
		double A;
		double B;
		double C;

	public:
		Line() : A(0), B(0), C(0) {}
		Line(const double _A, const double _B, const double _C) : A(_A), B(_B), C(_C) {}
		Line(const Point q, const Point w) {
			assert(q != w);
			A = w.y - q.y;
			B = q.x - w.x;
			C = w.x * q.y - w.y * q.x;
		}
		Line(const double k, const double b) {
			A = k;
			B = -1;
			C = b;
		}

		double get_A() const {
			return A;
		}

		double get_B() const {
			return B;
		}

		double get_C() const {
			return C;
		}

		bool contains(Point p) const {
			return eq(A * p.x + B * p.y + C, 0);
		}

		friend double distance(const Point, const Line);
		friend double distance(const Line, const Point);
		friend bool doesPointBelongToALine(const Point, const Line);
		friend bool doLinesIntersect(const Line, const Line);
	};

	double distance(const Point p, const Line l) {
		if (eq(l.A, 0) && eq(l.B, 0)) {
			if (eq(l.C, 0)) {
				return 0;
			}
			else {
				return 1e9; //no such line exists
			}
		}
		return std::abs(l.A * p.x + l.B * p.y + l.C) / sqrt(l.A * l.A + l.B * l.B);
	}

	double distance(const Line l, const Point p) {
		return distance(p, l);
	}

	bool doesPointBelongToALine(const Point p, const Line l) {
		return eq(l.A * p.x + l.B * p.y + l.C, 0);
	}

	bool doLinesIntersect(const Line l, const Line m) {
		if (eq(l.A * m.B, l.B * m.A)) {
			if (eq(l.C, m.C)) {
				return true; // the same line
			}
			else {
				return false; // collinear
			}
		}
		return true; //1 point of intersection
	}

	class Circle {
		Point center;
		double radius;

	public:
		Circle() : center({0, 0}), radius(0) {}
		Circle(Point p, double r) : center(p), radius(r) {}

		Point get_center() const {
			return center;
		}

		double get_radius() const {
			return radius;
		}

		friend bool doCirclesIntersect(const Circle, const Circle);
		friend bool doRectangleAndCircleIntersect(const Rectangle, const Circle);
		friend bool doSegmentAndCircleIntersect(const Segment, const Circle);
	};

	bool doCirclesIntersect(const Circle a, const Circle b) {
		return less(distance(a.center, b.center), a.radius + b.radius);
	}

	class Segment {
		Point l;
		Point r;

	public:
		Segment() : l({ 0, 0 }), r({ 0, 0 }) {}
		Segment(Point a, Point b) {
			if (leq(a.x, b.x)) {
				l = a;
				r = b;
			}
			else {
				l = b;
				r = a;
			}
		}

		friend class Line;

		Point get_l() const {
			return l;
		}

		Point get_r() const {
			return r;
		}

		friend double distance(const Point, const Segment);
		friend double distance(const Segment, const Point);
		friend double distance(const Segment, const Segment);
		friend bool doesPointBelongToASegment(const Point, const Segment);
		friend bool doSegmentAndCircleIntersect(const Segment, const Circle);
		friend bool doSegmentsIntersect(const Segment, const Segment);
		friend bool doSegmentAndRectangleIntersect(const Segment, const Rectangle);
	};

	bool doSegmentAndCircleIntersect(const Segment s, const Circle c) {
		return less(distance(s, c.center), c.radius);
	}

	bool doSegmentsIntersect(const Segment a, const Segment b) {
		double b1 = vector_product(a.r - a.l, b.l - a.l);
		double b2 = vector_product(a.r - a.l, b.r - a.l);
		double a1 = vector_product(b.r - b.l, a.l - b.l);
		double a2 = vector_product(b.r - b.l, a.r - b.l);
		if (eq(b1, 0) && eq(b2, 0)) { // a and b are on the same line
			if (eq(a.l.x, a.r.x)) { //vertical line
				double l1 = std::min(a.l.y, a.r.y);
				double r1 = std::max(a.l.y, a.r.y);

				double l2 = std::min(b.l.y, b.r.y);
				double r2 = std::max(b.l.y, b.r.y);
				//we need to see whether segments [l1, r1] and [l2, r2] intersect
				if (leq(r1, l2) || leq(r2, l1)) {
					return false;
				}
				else {
					return true;
				}
			}
			else { //if it's not vertical we can use the same logic for x-es
				double l1 = std::min(a.l.x, a.r.x);
				double r1 = std::max(a.l.x, a.r.x);

				double l2 = std::min(b.l.x, b.r.x);
				double r2 = std::max(b.l.x, b.r.x);

				if (leq(r1, l2) || leq(r2, l1)) {
					return false;
				}
				else {
					return true;
				}
			}
		}
		if (doesPointBelongToASegment(b.l, a) || doesPointBelongToASegment(b.r, a)) {
			return true;
		}
		if (doesPointBelongToASegment(a.l, b) || doesPointBelongToASegment(a.r, b)) {
			return true;
		}
		if (a1 * a2 < 0 && b1 * b2 < 0) {
			return true;
		}
		else {
			return false;
		}
	}

	double distance(const Point p, const Segment s) {
		if (s.l == s.r) {
			return distance(p, s.l);
		}
		if (eq(s.l.x, s.r.x)) {
			double low = std::min(s.l.y, s.r.y);
			double upper = std::max(s.l.y, s.r.y);
			if (less(p.y, low) || less(upper, p.y)) {
				return std::min(distance(p, s.l), distance(p, s.r));
			}
			else {
				return std::abs(p.x - s.l.x);
			}
		}
		else {
			Point ortToS = { -(s.r.y - s.l.y), (s.r.x - s.l.x) };
			if (vector_product(p - s.r, ortToS) > 0) {
				return distance(p, s.r);
			}
			if (vector_product(ortToS, p - s.l) > 0) {
				return distance(p, s.l);
			}
			return distance(p, Line(s.l, s.r));
		}
	}

	double distance(const Segment s, const Point p) {
		return distance(p, s);
	}

	double distance(const Segment s, const Segment t) {
		if (doSegmentsIntersect(s, t)) {
			return 0;
		}
		double res1 = std::min(distance(s.l, t), distance(s.r, t));
		double res2 = std::min(distance(t.l, s), distance(t.r, s));
		return std::min(res1, res2);
	}

	bool doesPointBelongToASegment(const Point p, const Segment s) {
		if (eq(vector_product(p - s.l, s.r - s.l), 0)) { //s.l, p and s.r are on the same line
			if (eq(s.l.x, s.r.x)) {
				return leq(std::min(s.l.y, s.r.y), p.y) && leq(p.y, std::max(s.l.y, s.r.y));
			}
			else {
				return leq(s.l.x, p.x) && leq(p.x, s.r.x);
			}
		}
		else {
			return false;
		}
	}

	class Rectangle {
		Point ld;//left-down
		Point ru;//right-up

	public:
		Rectangle() : ld({ 0, 0 }), ru({ 0, 0 }) {}
		Rectangle(Point a, Point b) {
			ld = min(a, b);
			ru = max(a, b);
		}

		Point get_ld() const {
			return ld;
		}

		Point get_ru() const {
			return ru;
		}

		friend bool doRectanglesIntersect(const Rectangle, const Rectangle);
		friend bool doesPointBelongToARectangle(const Point, const Rectangle);
		friend std::vector<Segment> BordersOfRectangle(const Rectangle);
		friend bool doRectangleAndCircleIntersect(const Rectangle, const Circle);
		friend bool doSegmentAndRectangleIntersect(const Segment, const Rectangle);
	};

	bool doRectanglesIntersect(const Rectangle A, const Rectangle B) {
		Point ld = max(A.ld, B.ld);
		Point ru = min(A.ru, B.ru);
		return less(ld.x, ru.x) && less(ld.y, ru.y);
	}

	bool doesPointBelongToARectangle(const Point p, const Rectangle r) {
		return r.ld <= p && p <= r.ru;
	}

	std::vector<Segment> BordersOfRectangle(const Rectangle r) {
		std::vector<Segment> res;
		res.push_back(Segment({ r.ld.x, r.ld.y }, { r.ru.x, r.ld.y }));//down-border
		res.push_back(Segment({ r.ru.x, r.ld.y }, { r.ru.x, r.ru.y }));//right-border
		res.push_back(Segment({ r.ru.x, r.ru.y }, { r.ld.x, r.ru.y }));//up-border
		res.push_back(Segment({ r.ld.x, r.ru.y }, { r.ld.x, r.ld.y }));//left-border
		return res;
	}

	bool doSegmentAndRectangleIntersect(const Segment s, const Rectangle r) {
		if (doesPointBelongToARectangle(s.l, r) || doesPointBelongToARectangle(s.r, r)) {
			return true;
		}
		std::vector<Segment> res = BordersOfRectangle(r);
		for (const Segment& res_s : res) {
			if (doSegmentsIntersect(s, res_s)) {
				return true;
			}
		}
	}

	bool doRectangleAndCircleIntersect(const Rectangle r, const Circle c) {
		if (doesPointBelongToARectangle(c.center, r)) {
			return true;
		}
		std::vector<Segment> res = BordersOfRectangle(r);
		for (const Segment& s : res) {
			if (doSegmentAndCircleIntersect(s, c)) {
				return true;
			}
		}
		return false;
	}

};