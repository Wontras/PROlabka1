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

vector<int> row_from_st_upper_point(const Matrix& matrix, int target_point) {
	vector<int> row(target_point);
	for (int i = 0; i < target_point; ++i) {
		row[i] = matrix[i][target_point];
	}
	return row;
}

vector<int> row_from_st_under_point(const Matrix& matrix, int target_point) {
	vector<int> row(matrix.size()-target_point-1);
	for (int i = target_point+1, j = 0; i < matrix.size(); ++i, ++j) {
		row[j] = matrix[i][target_point];
	}
	return row;
}

class MatrixTransposer {
public:
	MatrixTransposer(Matrix * input_matrix) : input_matrix(input_matrix), st_matrix(*input_matrix){}


	void operator()(const blocked_range<int>& r) const
	{
		int begin = r.begin(), end = r.end();
		
		vector<int> right_part = row_from_st_under_point(st_matrix, begin);
		vector<int> left_part = row_from_st_upper_point(st_matrix, begin);
		
		int i = 0;
		for (int i = 0; i < begin; ++i) {
			(*input_matrix)[begin][i] = left_part[i];
		}
		(*input_matrix)[begin][begin] = st_matrix[begin][begin];
		for (int i = begin+1; i < st_matrix.size(); ++i) {
			(*input_matrix)[begin][i] = right_part[i-begin-1];
		}
	}


private:
	Matrix* input_matrix;
	Matrix st_matrix;
};


int main() {
	srand(time(0));
	Matrix m(4, vector<int>(4));
	for (auto & i : m) {
		generate(i.begin(), i.end(), []() { return rand() % 20; });
	}
	cout << "Matrix before transposing: " << endl;
	for (auto i : m) {
		copy(i.begin(), i.end(), ostream_iterator<int>(cout, " "));
		cout << endl;
	}
	task_scheduler_init init(m.size());
	parallel_for(blocked_range<int>(0, m.size(), 1),MatrixTransposer(&m));
	cout << "Martix after transposing: " << endl;
	for (auto i : m) {
		copy(i.begin(), i.end(), ostream_iterator<int>(cout, " "));
		cout << endl;
	}

	return 0;
}