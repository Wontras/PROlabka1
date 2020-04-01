#define _CRT_SECURE_NO_WARNINGS
#include <mpi.h>
#include <iostream>
#include <vector>
#define MAX_MESSAGE_SIZE 100
#define BUFFER_SIZE 100

using namespace std;

void get_number_from_console(int *number) {
	cin >> *number;
}
int main(int argc, char * argv[]) {

	MPI_Status status;
	int number_from_console;
	const char * message = (const char *)"Whether 'tis nobler";
	int buffsersize = strlen(message) + 1;
	char readed_message[20];
	MPI_Init(&argc, &argv); int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0) {
		get_number_from_console(&number_from_console);
		MPI_Send((int*)&number_from_console, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);

		vector<char> buff(buffsersize + MPI_BSEND_OVERHEAD);
		char* pBuff = buff.data();
		MPI_Buffer_attach(pBuff, buffsersize + MPI_BSEND_OVERHEAD);
		char* strbuff = (char*)message;
		MPI_Bsend(strbuff, buffsersize, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
		MPI_Buffer_detach(&pBuff, &buffsersize);

		int message = 10;

		MPI_Ssend(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

		MPI_Recv(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
		cout << "#1 receive from #2: " << message << endl;

	}
	else if (rank == 1) {
		MPI_Send((int*)&number_from_console, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);

		MPI_Recv(&readed_message, buffsersize, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
		cout << "#2 receive from #1: " << readed_message << endl;
		vector<char> buff(buffsersize + MPI_BSEND_OVERHEAD);
		char* pBuff = buff.data();
		MPI_Buffer_attach(pBuff, buffsersize + MPI_BSEND_OVERHEAD);
		char *strbuffer = readed_message;
		MPI_Bsend(strbuffer, buffsersize, MPI_CHAR, 2, 0, MPI_COMM_WORLD);
		MPI_Buffer_detach(&pBuff, &buffsersize);

		int message;
		MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		cout << "#2 receive from #1: " << message << endl;
		for (int i = 0; i < 3; i += 2) {
			MPI_Ssend(&message, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	}
	else if (rank == 2) {
		int number_from_first_process;
		int number_from_second_process;
		MPI_Recv((int*)&number_from_first_process, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		MPI_Recv((int*)&number_from_second_process, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
		cout << "#3 receive from #1: " << number_from_first_process << endl;
		cout << "#3 receive from #2: " << number_from_second_process << endl;

		MPI_Recv(&readed_message, buffsersize, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &status);
		cout << "#3 receive from #2: " << readed_message << endl;

		int message;
		MPI_Recv(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
		cout << "#3 receive from #2: " << message << endl;
	}
	MPI_Finalize();

	return 0;
}