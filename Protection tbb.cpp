#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <stdlib.h>
#include <time.h>

#include <tbb/atomic.h>
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <tbb/task_scheduler_init.h>
using namespace std;
using namespace tbb;
using Matrix = vector<vector<int> >;


class MinMaxFinder
{
private:
	Matrix matrix;
	int* minimal;
	int* maximal;

public:
	
	MinMaxFinder(Matrix & matrix, int * minimal, int * maximal) : matrix(matrix), minimal(minimal), maximal(maximal){}

	void operator()(const blocked_range<int>& r) const
	{
		int begin = r.begin(), end = r.end();
		*minimal = min(*minimal, *min_element(matrix[begin].begin(), matrix[begin].end()));
		*maximal = max(*maximal, *max_element(matrix[begin].begin(), matrix[begin].end()));
	}
};

int main() {
	srand(time(0));
	Matrix m(4, vector<int>(4));
	for (auto & i : m) {
		generate(i.begin(), i.end(), []() { return rand() % 20; });
	}
	cout << "Matrix: " << endl;
	for (auto i : m) {
		copy(i.begin(), i.end(), ostream_iterator<int>(cout, " "));
		cout << endl;
	}
	int size = m.size();
	task_scheduler_init init(size);
	int minimal = m[0][0], maximal = m[0][0];
	MinMaxFinder s(m, &minimal, &maximal);
	parallel_for(blocked_range<int>(0, size, 1), s);
	cout << "Using tbb:\nminimal: " << minimal << "\nmaximal: " << maximal << endl;

	return 0;
}