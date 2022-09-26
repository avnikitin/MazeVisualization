#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <exception>
#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{
	if (argc > 3) {
		std::cout << "Only names of files to open and to save are required as parameters";
		exit(1);
	}
	if (argc <= 2) {
		std::cout << "Pass the name of files to read from and to write to as parameters";
		exit(1);
	}
	FILE* fopen = freopen(argv[1], "r", stdin);
	if (fopen == NULL) {
		std::cout << "Could not open file to read from";
		exit(1);
	}
	FILE* fclose = freopen(argv[2], "w", stdout);
	if (fclose == NULL) {
		std::cout << "Could not open file to write to";
		exit(1);
	}
	int szX, szY;
	std::cin >> szX >> szY;
	int N;
	std::cin >> N;
	std::vector <wchar_t> vertices(N);
	for (int i = 0; i < N; i++) {
		std::wcin >> vertices[i];
		std::wcout << vertices[i];
		if (i == N - 1) {
			std::cout << '\n';
		}
		else {
			std::cout << ' ';
		}
	}
	for (int i = 0; i < N; i++) {
		wchar_t vertex;
		int x, y;
		int entry, ext;
		int feeder, fake_feeder;
		int m; // edges
		std::wcin >> vertex;
		std::cin  >> x >> y >> entry >> ext
			>> feeder >> fake_feeder >> m;
		std::vector <wchar_t> adjacent(m);
		for (int j = 0; j < m; j++) {
			wchar_t to;
			int cnt;
			std::wcin >> to;
			std::cin >> cnt;
			adjacent[j] = to;
			int mid_x, mid_y;
			for (int k = 0; k < cnt; k++) {
				std::cin >> mid_x >> mid_y;
			}
		}

		std::wcout << vertex;
		std::cout << ": ";
		if (entry + ext + feeder == 0) {
			std::cout << "0 ";
		}
		else {
			std::cout << "1 " << entry << ' ' << ext << ' ' << feeder << ' ';
		}
		std::cout << "| (" << x << ", " << y << ")\n";
		for (int j = 0; j < m; j++) {
			std::wcout << adjacent[j];
			if (j == m - 1) {
				std::cout << '\n';
			}
			else {
				std::cout << ' ';
			}
		}
	}
	//then come the walls, they do not matter for Oleg
	return 0;
}
