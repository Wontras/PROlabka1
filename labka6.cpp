#include <vector>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <omp.h>

using namespace std;

omp_lock_t for_generate_matrix;
omp_lock_t for_count_numbers;


template<class T>
vector<vector<T> > generate_matrix(int height, int width) {
	vector<vector<T> > matrix(height);
	omp_init_lock(&for_generate_matrix);
	#pragma omp parallel num_threads(height) shared(matrix)
	#pragma omp for schedule(dynamic)
	for (int i = 0; i < height; ++i) {
		omp_set_lock(&for_generate_matrix);
		matrix[i].resize(width);
		generate(matrix[i].begin(), matrix[i].end(), []() {return (rand() % 322 - 166) / 4; });
		omp_unset_lock(&for_generate_matrix);
	}
	omp_destroy_lock(&for_generate_matrix);
	return matrix;
}

vector<int> get_number_of_positive_values(vector<vector<int> > matrix) {
	int m = matrix.size();
	vector<int> positive_in_rows(m);
	omp_init_lock(&for_count_numbers);
#pragma omp parallel num_threads(m) shared(matrix)
	{
#pragma omp for schedule(static, 2)
		
		for (int i = 0; i < m; ++i)
		{
			omp_set_lock(&for_count_numbers);
			positive_in_rows[i] = count_if(matrix[i].begin(), matrix[i].end(), [](int n) {return n > 0; });
			omp_unset_lock(&for_count_numbers);
		}
	}
	omp_destroy_lock(&for_count_numbers);
	return positive_in_rows;
}

void first_task(int heigth, int width) {
	auto matrix = generate_matrix<int>(heigth, width);

	int m = heigth;

	cout << "Matrix is:" << endl;
	for (auto i : matrix) {
		copy(i.begin(), i.end(), ostream_iterator<int>(cout, "\t"));
		cout << endl;
	}
	auto positive_in_rows = get_number_of_positive_values(matrix);

	cout << "Number of positive in all rows is: " << endl;
	copy(positive_in_rows.begin(), positive_in_rows.end(), ostream_iterator<int>(cout, ", "));
}


int main() {
	cout << "First task:" << endl << endl;
	first_task(8, 5);
	int i;
	cin >> i;
	return 0;
}