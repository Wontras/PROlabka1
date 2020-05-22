#include <vector>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <omp.h>


using namespace std;

vector<char> generate_char_array(int size) {
	vector<char> random_char_array(size);

	for (auto & i : random_char_array) {
		i = rand() % 93 + 33;
	}

	return random_char_array;
}

template<class T>
vector<vector<T> > generate_matrix(int height, int width) {
	vector<vector<T> > matrix(height);

	for (auto &i : matrix) {
		i.resize(width);
		generate(i.begin(), i.end(), []() {return (rand() % 322 - 166)/4; });
	}

	return matrix;
}

vector<int> get_number_of_positive_values(vector<vector<int> > matrix) {
	int m = matrix.size();
	vector<int> positive_in_rows(m);
	#pragma omp parallel num_threads(m) shared(matrix)
	{
		//#pragma omp for schedule(guided, 2)
		#pragma omp for schedule(static, 2)
		for (int i = 0; i < m; ++i)
		{
			positive_in_rows[i] = count_if(matrix[i].begin(), matrix[i].end(), [](int n) {return n > 0; });
		}
	}
	return positive_in_rows;
}

void first_task(int heigth, int width) {
	auto matrix = generate_matrix<int>(heigth, width);
	
	int m = heigth;
	#pragma omp parallel
	{
		#pragma omp single
		{
			cout << "Gladkiy O.V." << endl;
		}
	}
	cout << "Matrix is:" << endl;
	for (auto i : matrix) {
		copy(i.begin(), i.end(), ostream_iterator<int>(cout, "\t"));
		cout << endl;
	}
	auto positive_in_rows = get_number_of_positive_values(matrix);

	cout << "Number of positive in all rows is: " << endl;
	copy(positive_in_rows.begin(), positive_in_rows.end(), ostream_iterator<int>(cout, ", "));
}

void second_task() {
	auto random_char_array = generate_char_array(100);
	cout << "String is: " << endl;
	copy(random_char_array.begin(), random_char_array.end(), ostream_iterator<char>(cout, ""));
	cout << endl << "count of symbol \'A\' is " << count_if(random_char_array.begin(), random_char_array.end(), [](char i) {return i == 'A'; });
	
	int m = 5, n = 5;
	auto matrix = generate_matrix<float>(m, n);
	cout << endl << "Matrix before:" << endl;
	for (auto i : matrix) {
		copy(i.begin(), i.end(), ostream_iterator<int>(cout, "\t"));
		cout << endl;
	}
	#pragma omp parallel num_threads(m) shared(matrix)
	{
		#pragma omp for schedule (guided, 2)
		for (int i = 0; i < m; i++)
		{
			sort(matrix[i].begin(), matrix[i].end());
		}
	}
	cout << endl << "Matrix after:" << endl;
	for (auto i : matrix) {
		copy(i.begin(), i.end(), ostream_iterator<int>(cout, "\t"));
		cout << endl;
	}
}

int main() {
	cout << "First task:" << endl << endl;
	first_task(7, 10);
	cout << endl << endl << "Second task: " << endl << endl;
	second_task();
	return 0;
}