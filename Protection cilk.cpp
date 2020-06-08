#include <cilk/cilk.h>
#include <cilk/reducer.h>
#include <cilk/reducer_min_max.h>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <time.h>
#include <math.h>
#include <string>
#include <vector>
using namespace std;
using Matrix = vector<vector<int> >;

pair<int, int> minimal_maximal(const Matrix& matrix)
{
	int size = matrix.size();
	vector<int> minimal_in_rows(size);
	vector<int> maximal_in_rows(size);
	for (int j = 0; j < size; ++j) {
		cilk::reducer< cilk::op_max<int> > max_reducer;
		cilk::reducer< cilk::op_min<int> > min_reducer;
		cilk_for(int i = 0; i < size; ++i)
		{
			max_reducer->calc_max(matrix[j][i]);
			min_reducer->calc_min(matrix[j][i]);
		}
		minimal_in_rows[j] = min_reducer.get_value();
		maximal_in_rows[j] = max_reducer.get_value();
	}
	cilk::reducer< cilk::op_max<int> > max_reducer;
	cilk::reducer< cilk::op_min<int> > min_reducer;
	cilk_for(int i = 0; i < size; ++i)
	{
		max_reducer->calc_max(maximal_in_rows[i]);
		min_reducer->calc_min(minimal_in_rows[i]);
	}
	copy(minimal_in_rows.begin(), minimal_in_rows.end(), ostream_iterator<int>(cout, " "));
	return make_pair(min_reducer.get_value(), max_reducer.get_value());
}

Matrix create_matrix(int size){
	Matrix matrix(size);
	for_each(matrix.begin(), matrix.end(), [&](vector<int> & i) {i.resize(size); });

	for (auto & i : matrix) {
		generate(i.begin(), i.end(), []() {return rand() % 100; });
	}
	return matrix;
}

void print_matrix(const Matrix& matrix) {
	for (auto i : matrix) {
		copy(i.begin(), i.end(), ostream_iterator<int>(cout, " "));
		cout << endl;
	}
}
int main(){
	srand(time(0));
	int size;
	cout << "Size of matrix: ";
	cin >> size;
	Matrix a = create_matrix(size);
	cout << "Matrix: " << endl;
	print_matrix(a);
	auto b = minimal_maximal(a);
	cout << endl << "Min: " << b.first << "\t" << "Max: " << b.second << endl;

	return 0;
}