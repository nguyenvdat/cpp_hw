#include <iostream>
#include <cmath>
#include <numeric>
#include <algorithm>
using std :: cout; using std :: endl;

int main () {
	int size = 10 * 1024 / 8; // Produce 100KB
	int i = 0;
	while (true) {
		double data[(i+1)*size];
		std::fill(data , data + (i + 1)*size , 1.23);
		std::accumulate(data, data + (i + 1)*size, 0);

		cout << "Iteration: " << i << " allocated memory: " << (i + 1) * 8 * size / 1024 / 1024 << " MB" << endl;
		i++;
	}
	return 0;
}