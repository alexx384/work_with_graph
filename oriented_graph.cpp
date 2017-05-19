#include "oriented_graph.h"

using namespace std;

void oriented_graph::adjacency_list(std::ifstream *work_file, std::vector<std::vector <int>> *matrix)
{
	int count_row = 0;
	char c;
	while (!work_file->eof())
	{
		c = work_file->get();
		if (c == '\n')
		{
			++count_row;
		}
	}
	++count_row;


	work_file->clear();
	work_file->seekg(ios_base::beg);

	matrix->resize(count_row, vector<int>(count_row));

	int cur_row = -1;
	char buf[5] = { 0 };
	int *ptr = (int*)buf;
	while (!work_file->eof())
	{
		(*work_file) >> buf;

		if (buf[0] == '{')
		{
			++cur_row;
		}
		else {
			for (int i = 0; i < 4; ++i)
			{
				if (buf[i] == '}')
				{
					buf[i] = 0;
					break;
				}
			}
			/*if (atoi(buf) - 1 == cur_row)
			matrix->at(cur_row).at(atoi(buf) - 1) += 1;*/

			matrix->at(cur_row).at(atoi(buf) - 1) += 1;
		}
		*ptr = 0;
	}
}

void oriented_graph::show_degree_of(int number_of_vertex)
{
	int vert = get_count_vertex();

	if (number_of_vertex < 1 || number_of_vertex > vert)
	{
		cout << "Error: unable find vertex" << endl;
		return;
	}

	number_of_vertex = transform_num(number_of_vertex);
	if (number_of_vertex == ERROR)
	{
		return;
	}
	cout << "indegree = " << get_indegree_of(number_of_vertex) << ", ";
	cout << "Outdegree = " << get_outdegree_of(number_of_vertex) << endl;
}

void oriented_graph::show_degree_sequence()
{
	int size = user_vert.size();

	for (int i = 0; i < size; ++i)
	{
		cout << user_vert[i].num << ") ";
		show_degree_of(user_vert[i].num);
	}
}

int oriented_graph::get_outdegree_of(int number_of_vertex)
{
	int vert = basic_graph::get_count_vertex();

	int counter = 0;

	for (int col = 0; col < vert; ++col)
	{
		if (basic_graph::matrix.at(number_of_vertex).at(col) != 0)
			++counter;
	}
	return counter;
}

int oriented_graph::get_indegree_of(int number_of_vertex)
{
	int vert = basic_graph::get_count_vertex();

	int counter = 0;

	for (int row = 0; row < vert; ++row)
	{
		if (basic_graph::matrix.at(row).at(number_of_vertex) != 0)
			++counter;
	}
	return counter;
}

void oriented_graph::make_graph_subdivision(int from, int to)
{
	int vert = get_count_vertex();
	if ((from > vert) || (to > vert) || (from < 1) || (to < 1))
	{
		cout << "The value is out of range" << endl;
		return;
	}

	from = transform_num(from);
	to = transform_num(to);
	if ((from == ERROR) || (to == ERROR))
	{
		return;
	}

	if (basic_graph::matrix.at(from).at(to) == 0)
	{
		cout << "Error" << endl;
		return;
	}

	if (from == to)
	{
		matrix.at(from).at(to) -= 1;
		int num = get_unused_user_num();
		add_vertex(num);
		num = transform_num(num);
		matrix.at(from).at(num) += 1;
		return;
	}

	int num = get_unused_user_num();
	add_vertex(num);
	num = transform_num(num);

	basic_graph::matrix.at(from).at(to) -= 1;

	basic_graph::matrix.at(from).at(num) += 1;
	basic_graph::matrix.at(num).at(to) += 1;
}

vector<vector<int>>* oriented_graph::get_matrix()
{
	return &(basic_graph::matrix);
}

oriented_graph::oriented_graph(int type_of_graph, char * file_name)
{
	ifstream work_file(file_name);

	basic_graph::is_oriented = 1;

	if (!work_file.is_open())
	{
		cout << "Error openning file" << endl;
		system("pause");
		exit(-1);
	}

	switch (type_of_graph)
	{
	case ADJACENCY_MATR:
	{
		basic_graph::adjacency_matrix(&work_file, &matrix);
	}break;
	case INCIDENCE_MATR:
	{
		basic_graph::incidence_matrix(&work_file, &matrix);
	}break;
	case ADJACENCY_LIST:
	{
		oriented_graph::adjacency_list(&work_file, &matrix);
	}break;
	}

	user_vert.resize(matrix.size());
	for (int i = 0; i < matrix.size(); ++i)
	{
		user_vert[i].num = i + 1;
		user_vert[i].cur_num = i;
	}

	//========== Write to vector ==========

	work_file.close();
}