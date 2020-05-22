#include <mpi.h>
#include <iterator>
#include <array>
#include <iostream>
#include <vector>

using namespace std;

void first_task(MPI_Group global_group, int rank, int size) {
	if (size == 4) {
		MPI_Group first_group, second_group, groups_union;
		vector<int> first_group_ranks = { 0, 2 };
		vector<int> second_group_ranks = { 2, 3 };
		int rank_in_group;
		MPI_Group_incl(global_group, first_group_ranks.size(), first_group_ranks.data(), &first_group);
		MPI_Group_incl(global_group, second_group_ranks.size(), second_group_ranks.data(), &second_group);
		MPI_Group_union(first_group, second_group, &groups_union);
		MPI_Group_rank(groups_union, &rank_in_group);
		if (rank == 0) {
			cout << "Union of groups ";
			copy(first_group_ranks.begin(), first_group_ranks.end(), ostream_iterator<int>(cout, " "));
			cout << " and ";
			copy(second_group_ranks.begin(), second_group_ranks.end(), ostream_iterator<int>(cout, " "));
		}
		if (rank_in_group >= 0) {
			cout << endl;
			cout << rank << " what in global group have rank " << rank_in_group << endl;
		}


		MPI_Group_free(&first_group);
		MPI_Group_free(&second_group);
		MPI_Group_free(&groups_union);
	}
}


void second_task(MPI_Group global_group, MPI_Comm global_communitator, int rank, int size) {
	MPI_Group local_group;
	MPI_Comm local_communicator;
	vector<int> group_ranks = { 1, 2, 6, 7, 8 };
	MPI_Group_incl(global_group, group_ranks.size(), group_ranks.data(), &local_group);
	MPI_Comm_create(global_communitator, local_group, &local_communicator);

	int message1, message2;
	int rank_in_group;

	MPI_Group_rank(local_group, &rank_in_group);
	if (rank_in_group >= 0) {
		if (rank_in_group == 0) {
			message1 = 1000;
			cout << "rank#" << rank_in_group << " send message " << message1 << endl;
		}
		if (rank_in_group == group_ranks.size() - 1) {
			message2 = 2000;
			cout << "rank#" << rank_in_group << " send message " << message2 << endl;
		}
		MPI_Bcast(&message1, 1, MPI_INT, 0, local_communicator);
		MPI_Bcast(&message2, 1, MPI_INT, group_ranks.size() - 1, local_communicator);
		if (rank_in_group % 2) {
			cout << "rank#" << rank_in_group << " receive messages " << message1 << " and " << message2 << endl;
		}
		MPI_Group_free(&local_group);
		MPI_Comm_free(&local_communicator);
	}
	
}
int main(int argc, char *argv[]) {
	MPI_Comm global_communicator; MPI_Group global_group;
	int global_rank, global_size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &global_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &global_size);
	MPI_Comm_dup(MPI_COMM_WORLD, &global_communicator);
	MPI_Comm_group(global_communicator, &global_group);
	if (global_rank == 0) {
		cout << "First task:" << endl;
	}
	if (global_rank < 4) {
		first_task(global_group, global_rank, 4);
	}
	if (global_rank == 0) {
		cout << "Second task:" << endl;
	}
	if (global_size == 9) {
		second_task(global_group, global_communicator, global_rank, global_size);
	}
	MPI_Group_free(&global_group);
	MPI_Comm_free(&global_communicator);
	MPI_Finalize();
}