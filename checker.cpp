#define _CRT_SECURE_NO_WARNINGS
#include "geometry.h"
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <string>

using namespace geometry;

std::string make_suffix(int x) {
	if (10 <= x && x <= 20) {
		return "th";
	}
	switch (x % 10) {
	case 1:
		return "st";
	case 2:
		return "nd";
	case 3:
		return "rd";
	default:
		return "th";
	}
}

std::string ord(int x) {
	return std::to_string(x) + "'" + make_suffix(x);
}

int main(int argc, char *argv[]) {

	/// syntax issues 

	setlocale(LC_ALL, "Russian");
	if (argc != 2) {
		std::cout << "Just one parameter required";
		exit(1);
	}
	FILE* fopen = freopen(argv[1], "r", stdin);
	if (fopen == NULL) {
		std::cout << "Could not open file to read from";
		exit(1);
	}
	int szX, szY;
	if (!(std::cin >> szX >> szY)) {
		std::cout << "First two values have to be sizes of a maze";
		exit(1);
	}
	int N;
	if (!(std::cin >> N)) {
		std::cout << "Could not read the number of vertices";
		exit(1);
	}
	std::vector <std::pair <Circle, wchar_t>> vertices;
	std::vector <std::pair <Segment, std::wstring>> edges;
	std::set<wchar_t> names;
	std::set<wchar_t> all_names;
	for (int i = 1; i <= N; i++) {
		wchar_t c;
		if (!(std::wcin >> c)) {
			std::cout << "Could not read " << ord(i) << " letter in vertices list";
			exit(1);
		}
		if (names.count(c) == 1) {
			std::wstring error = L"vertice ";
			error += c;
			error += L" was found more than once in list of vertices in input";
			std::wcout << error;
			exit(1);
		}
		names.insert(c);
		all_names.insert(c);
	}
	std::map <wchar_t, Point> coord;
	std::vector <int> entries;
	int exits = 0;
	for (int i = 1; i <= N; i++) {
		wchar_t c;
		std::string Error = "Could not read " + ord(i) + "vertex description: ";
		if (!(std::wcin >> c)) {
			std::cout << Error << "name";
			exit(1);
		}
		if (names.count(c) == 0) {
			std::wstring error = L"vertice ";
			error += c;
			error += L" was not found in list of vertices but was found later";
			std::wcout << error;
			exit(1);
		}
		else {
			names.erase(c);
		}
		int x, y;
		if (!(std::cin >> x >> y)) {
			std::cout << Error << "coordinates";
			exit(1);
		}
		coord[c] = Point(x, y);
		int entry, ext, feeder, fake_feeder;
		if (!(std::cin >> entry >> ext >> feeder >> fake_feeder)) {
			std::cout << Error;
			std::wcout << L"ÁÇÝ: could not read";
			exit(1);
		}
		if (entry < 0 || 1 < entry || ext < 0 || 1 < ext || feeder < 0 || 1 < feeder
			|| fake_feeder < 0 || 1 < fake_feeder) {
			std::cout << Error;
			std::wcout << L"ÁÇÝ: have to be either 0 or 1";
			exit(1);
		}
		if (entry + ext + feeder + fake_feeder > 1) {
			if (feeder + fake_feeder > 0) {
				std::cout << Error;
				std::wcout << L"ÁÇÝ: only one variable can be set to one";
				exit(1);
			}
		}
		if (entry) {
			entries.push_back(i);
		}
		exits += ext;
		vertices.push_back(std::make_pair(Circle(Point(x, y), 0.5), c));
		int m;
		if (!(std::cin >> m)) {
			std::cout << Error << "number of edges";
			exit(1);
		}
		for (int j = 1; j <= m; j++) {
			wchar_t to;
			std::string Error2 = ord(j) + " letter in edges: ";
			if (!(std::wcin >> to)) {
				std::cout << Error << Error2 << "it's name";
				exit(1);
			}
			if (all_names.count(to) == 0) {
				std::wcout << L"vertex " << to << L" is in edges list, but not in the list of vertices";
				exit(1);
			}
			Point cur = Point(x, y);
			int mid;
			if (!(std::cin >> mid)) {
				std::cout << Error << Error2 << "number of intermediate points";
				exit(1);
			}
			std::wstring edge_name = L"";
			edge_name += c;
			edge_name += to;
			for (int k = 1; k <= mid; k++) {
				int to_x, to_y;
				if (!(std::cin >> to_x >> to_y)) {
					std::cout << Error << Error2 << ord(k) << " intermediate point";
					exit(1);
				}
				edges.push_back(std::make_pair(Segment(cur, Point(to_x, to_y)), edge_name));
				cur = Point(to_x, to_y);
			}
			if (mid != -1) {
				edges.push_back(std::make_pair(Segment(cur, Point(12345, 12345)), edge_name));
			}
		}
	}
	if (exits == 0) {
		std::cout << "There's no exit in a maze";
		exit(1);
	}
	if (entries.empty()) {
		std::cout << "There's no entry in a maze";
		exit(1);
	}
	if (entries.size() > 1) {
		std::cout << "There're more than one entry in a maze: ";
		for (int i : entries) {
			std::cout << i << ' ';
		}
		exit(1);
	}
	int wallsN;
	if (!(std::cin >> wallsN)) {
		std::cout << "Could not read number of walls";
		exit(1);
	}
	std::vector <std::pair <Rectangle, int>> walls;
	for (int i = 1; i <= wallsN; i++) {
		int a, b, c, d;
		if (!(std::cin >> a >> b >> c >> d)) {
			std::cout << "Could not read " << ord(i) << " wall description";
			exit(1);
		}
		walls.push_back(std::make_pair(Rectangle(Point(a, b), Point(c, d)), i));
	}
	
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			if (doCirclesIntersect(vertices[i].first, vertices[j].first)) {
				std::wcout << "vertices " << vertices[i].second << " and "
					<< vertices[j].second << " do intersect";
				exit(1);
			}
		}
	}

	wchar_t rem;
	if (std::wcin >> rem) {
		std::cout << "Something after the walls";
		exit(1);
	}

	/// Logical issues

	for (int i = 0; i < walls.size(); i++) {
		for (int j = i + 1; j < walls.size(); j++) {
			if (doRectanglesIntersect(walls[i].first, walls[j].first)) {
				std::cout << ord(i + 1) << " and " << ord(j + 1) << " walls do intersect";
				exit(1);
			}
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < walls.size(); j++) {
			if (doRectangleAndCircleIntersect(walls[j].first, vertices[i].first)) {
				std::cout << ord(j + 1) << " wall and vertex ";
				std::wcout << vertices[i].second << L" do intersect";
				exit(1);
			}
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j + 1 < N; j++) {
			if (doCirclesIntersect(vertices[i].first, vertices[j].first)) {
				std::wcout << L"vertices " << vertices[i].second << " and "
					<< vertices[j].second << L" do intersect";
				exit(1);
			}
		}
	}
	
	for (int i = 0; i < edges.size(); i++) {
		if (i == 3) {
			i = 3;
		}
		Point to = edges[i].first.get_r();
		if (to.get_x() == 12345) {
			wchar_t req = edges[i].second[1];
			to = coord[req];
		}
		for (int j = 0; j < walls.size(); j++)
			if (doSegmentAndRectangleIntersect(Segment(edges[i].first.get_l(), to), walls[j].first)) {
				std::wcout << edges[i].second;
				std::cout << edges[i].first.get_l().get_x() << ' ' << edges[i].first.get_l().get_y()<<'\n';
				std::cout << to.get_x() << ' ' << to.get_y() << '\n';;
				std::cout << '\n';
				std::cout << ord(j + 1) << " wall and path between vertices ";
				std::wcout << edges[i].second[0] << L" and " << edges[i].second[1] << L" do intersect";
				exit(1);
		}
	}
	std::cout << "Everything is fine!";
}