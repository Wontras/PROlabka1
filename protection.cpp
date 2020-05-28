#include <mpi.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <time.h>

using namespace std;

class Matrix {
public:
	Matrix(const Matrix&m) : content(m.content) {}
	Matrix(int n, int m) {
		content.resize(n);
		for_each(content.begin(), content.end(), [&](vector<int> &i) {i.resize(m); });
	}
	Matrix(initializer_list<vector<int> > content) : content(content) {}

	void random_generate() {
		for (auto & i : content) {
			generate(i.begin(), i.end(), []() {return rand() % 20; });
		}
	}

	vector<int>& operator[](int index) { return content[index]; }

	Matrix operator * (const Matrix &another) {
		if (content[0].size() != another.content.size()) {
			return Matrix(0, 0);
		}
		Matrix result(content.size(), another.content[0].size());
		#pragma omp parallel num_threads(content.size()-1) shared(result)
		{
			#pragma omp for 
			for (int i = content.size() - 1; i >= 0; --i) {
				for (int j = another.content[0].size() - 1; j >= 0; --j) {
					for (int k = content[i].size() - 1; k >= 0; --k) {
						result[i][j] += content[i][k] * another.content[k][j];
					}
				}
			}
		}

		return result;
	}
	
	void print() {
		for (auto i : content) {
			copy(i.begin(), i.end(), ostream_iterator<int>(cout, "\t"));
			cout << endl;
		}
	}
private:
	vector<vector<int> > content;
};


int main() {
	srand(time(0));
	Matrix a(4, 4);
	a.random_generate();
	Matrix b({ {1, 2, 3, 4},
			   {5, 6, 7, 8},
		       {9, 10, 11, 12},
		       {13, 14, 15, 16} });
	cout << "First matrix: " << endl;
	a.print();
	
	cout << "Second matrix: " << endl;
	b.print();

	Matrix c = a * b;
	cout << "Result matrix: " << endl;
	c.print();
	system("pause");
	return 0;
}