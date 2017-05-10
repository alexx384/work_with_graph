#include "normal_graph.h"

using namespace std;

normal_graph::normal_graph(int type_of_graph, char * file_name)
{
	ifstream work_file(file_name);

	basic_graph::is_oriented = 0;

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
		normal_graph::adjacency_list(&work_file, &matrix);
	}break;
	}

	//========== Write to vector ==========

	work_file.close();
}

void normal_graph::delete_edge(int from, int to)
{
	int vert = get_count_vertex();

	if ((from > vert) || (to > vert) || (from < 1) || (to < 1))
	{
		cout << "Error, the value is out of range" << endl;
		return;
	}

	--from;
	--to;

	if (basic_graph::matrix.at(from).at(to) != 0 && basic_graph::matrix.at(to).at(from) != 0)
	{
		basic_graph::matrix.at(from).at(to) -= 1;
		basic_graph::matrix.at(to).at(from) -= 1;
	}else {
		cout << "Couldn't delete unknow nonexistent edge" << endl;
	}
}

void normal_graph::add_edge(int from, int to)
{
	int vert = get_count_vertex();

	if ((from > vert) || (to > vert) || (from < 1) || (to < 1))
	{
		cout << "Error, the value is out of range" << endl;
		return;
	}

	--from;
	--to;

	basic_graph::matrix.at(from).at(to) += 1;
	basic_graph::matrix.at(to).at(from) += 1;
}

void normal_graph::show_length_of_radius()
{
	vector<vector<int>> *radius = get_length_of_radius();

	for (int i = 0; i < radius->size(); i++)
	{
		cout << "The radius is " << radius->at(i).back() << endl;
	}
	delete radius;
}

void normal_graph::show_length_of_diameter()
{
	vector<vector<int>> *diameter = get_length_of_diameter();

	for (int i = 0; i < diameter->size(); i++)
	{
		cout << "The diameter is " << diameter->at(i).back() << endl;
	}
	delete diameter;
}

void normal_graph::Prims_algorithm()
{
	int vert = basic_graph::get_count_vertex();
	vector<vector<int>> temp_matrix(vert, vector<int>(vert));

	const int INF = 1000000000; // значение "бесконечность"

	for (int row = 0; row < vert; ++row)
	{
		for (int col = 0; col < vert; ++col)
		{
			if (matrix[row][col] == 0)
			{
				temp_matrix[row][col] = INF;
			} else {
				temp_matrix[row][col] = matrix[row][col];
			}
		}
	}

	int n = vert;

								// алгоритм
	vector<bool> used(n, false);
	vector<int> dist(n, INF);
	vector <int> path(n);
	dist[0] = 0;
	int path_len = 0;
	for (int i = 0; i < n; ++i)
	{
		int min_dist = INF;
		int row;
		for (int num_vert = 0; num_vert < n; ++num_vert)
		{
			//Ищем минимальное ребро у неиспользованных вершин
			if (!used[num_vert] && (dist[num_vert] < min_dist))
			{
				min_dist = dist[num_vert];
				row = num_vert;
			}
		}

		//Подсчёт результатов
		path_len += min_dist;
		path[i] = row;

		used[row] = true;
		//Пересчитываем минимальные дистанции
		for (int col = 0; col < n; ++col)
		{
			dist[col] = min(dist[col], temp_matrix[row][col]);
		}	
	}

	cout << path_len << endl;
	//basic_graph::show_external_matrix(&temp_matrix);

	for (int i = 0; i < vert; ++i)
	{
		cout << path[i]+1 << " -> ";
	}

}

void normal_graph::adjacency_list(std::ifstream *work_file, std::vector<std::vector <int>> *matrix)
{
	cout << "Here" << endl;
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
			if (atoi(buf) - 1 == cur_row)
				matrix->at(cur_row).at(atoi(buf) - 1) += 1;
			
			matrix->at(cur_row).at(atoi(buf) - 1) += 1;
		}
		*ptr = 0;
	}
}

void normal_graph::show_degree_of(int number_of_vertex)
{
	int vert = get_count_vertex();

	if (number_of_vertex < 1 || number_of_vertex > vert)
	{
		cout << "Error: unable find vertex" << endl;
		return;
	}
	--number_of_vertex;
	cout << "Degree = " << basic_graph::get_degree_of(number_of_vertex) << endl;
}

void normal_graph::show_degree_sequence()
{
	int size = matrix.size() + 1;

	for (int i = 1; i < size; ++i)
	{
		cout << i << ") ";
		show_degree_of(i);
	}
}

void normal_graph::make_graph_subdivision(int from, int to)
{
	int vert = get_count_vertex();

	if((from > vert) || (to > vert) || (from < 1) || (to < 1))
	{
		cout << "The value is out of range" << endl;
		return;
	}

	--from;
	--to;

	if (basic_graph::matrix.at(from).at(to) == 0 || basic_graph::matrix.at(to).at(from) == 0)
	{
		cout << "Error" << endl;
		return;
	}

	if (from == to)
	{
		matrix.at(from).at(to) -= 2;
		return;
	}

	basic_graph::add_vertex();

	
	basic_graph::matrix.at(from).at(to) -= 1;
	basic_graph::matrix.at(to).at(from) -= 1;

	vert = basic_graph::get_count_vertex() - 1;

	if (from != to)
	{
		basic_graph::matrix.at(from).at(vert) += 1;
		basic_graph::matrix.at(vert).at(from) += 1;
	}

	basic_graph::matrix.at(to).at(vert) += 1;
	basic_graph::matrix.at(vert).at(to) += 1;
}

vector<vector<int>>* normal_graph::get_matrix()
{
	return &(basic_graph::matrix);
}