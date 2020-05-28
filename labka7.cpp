#include <cilk/cilk.h>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <time.h>
#include <math.h>
#include <string>
#include <vector>
using namespace std;
using Matrix = vector<vector<int> >;

Matrix transpose(const Matrix& matrix)
{
	Matrix temp(matrix);
	int size = matrix.size();
	cilk_for (int i = 0; i < size; i++){
		for (int j = i; j < size; j++) {
			swap(temp[i][j], temp[j][i]);
		}
	}
	return temp;
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
	cout << "Matrix before transposing: " << endl;
	print_matrix(a);
	cout << endl << "Matrix after transposing: " << endl;
	Matrix b = transpose(a);
	print_matrix(b);

	return 0;
}