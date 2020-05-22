#include <mpi.h>
#include <iostream>
#include <iterator>
#include <fstream>
#include <string.h>
#include <vector>
using namespace std;
MPI_Status status;
MPI_Request request;
int arr[20] = { 9, 8, 7, 6, 2, 4, 4, 2, 1, 0, 2, 8, 7, 6, 1, 4, 2, 3, 1, 0 };
int msg[4];
int a[20];
int bf[8];
int *buf;
int t = 30;
void bubblesort(int *a, int n)
{
	int i, j;
	for (i = n - 1; i >= 0; i--)
	{
		for (j = 0; j < i; j++)
		{
			if (a[j] > a[j + 1])
				swap(a[j], a[j + 1]);
		}
	}
}

void first_task(int rank, int size) {
	int message;
	if (rank % 3 == 0) {
		message = rand() % 333;
	}

	MPI_Bcast(&message, 1, MPI_INT, 3, MPI_COMM_WORLD);

	if (rank % 2 == 0) {
		cout << "rank#" << rank << " take message " << message << endl;
	}
}
void second_task(int rank, int p1, int p2)
{
	if (rank == p2)
	{
		MPI_Issend(msg, 4, MPI_INT, p1, 0, MPI_COMM_WORLD, &request);
		MPI_Wait(&request, &status);
		MPI_Irecv(msg, 4, MPI_INT, p1, 0, MPI_COMM_WORLD, &request);
		MPI_Wait(&request, &status);
	}
	if (rank == p1)
	{
		MPI_Irecv(bf, 4, MPI_INT, p2, 0, MPI_COMM_WORLD, &request);
		MPI_Wait(&request, &status);
		for (int i = 4; i < 8; i++)
		{
			bf[i] = msg[i - 4];
		}
		bubblesort(bf, 8);
		for (int i = 0; i < 4; i++)
		{
			msg[i] = bf[i];
		}
		MPI_Issend(bf + 4, 4, MPI_INT, p2, 0, MPI_COMM_WORLD, &request);
		MPI_Wait(&request, &status);
	}
}
int main(int argc, char *argv[])
{
	int rank;
	int size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (size == 5)
	{
		if (rank == 0) {
			cout << "first task: " << endl;
		}
		first_task(rank, size);
		MPI_Scatter(arr, 4, MPI_INT, msg, 4, MPI_INT, 0, MPI_COMM_WORLD);
		if (rank == 0) {
			cout << endl << "second task: " << endl;
		}
		for (int i = 0; i < size; i++) {
			for (int j = i + 1; j < size; j++) {
				second_task(rank, i, j);
			}
		}

		MPI_Gather(msg, 4, MPI_INT, a, 4, MPI_INT, 0, MPI_COMM_WORLD);
		if (rank == 0)
		{
			for (int i = 0; i < 20; i++)
			{
				cout << arr[i] << " ";
			}
			cout << endl;
			for (int i = 0; i < 20; i++)
			{
				cout << a[i] << " ";
			}
		}
	}
	MPI_Finalize();
	return 0;
}