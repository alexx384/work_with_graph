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

	user_vert.resize(matrix.size());
	for (int i = 0; i < matrix.size(); ++i)
	{
		user_vert[i].num = i + 1;
		user_vert[i].cur_num = i;
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

	from = transform_num(from);
	to = transform_num(to);
	if ((from == ERROR) || (to == ERROR))
	{
		return;
	}

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

	from = transform_num(from);
	to = transform_num(to);

	if ((from == ERROR) || (to == ERROR))
	{
		return;
	}

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

void normal_graph::build_ostov_prims()
{
	int vert = basic_graph::get_count_vertex();

	vector<vector<int>> temp_matrix(vert, vector<int>(vert));
	vector<vector<int>> result_matrix(vert, vector<int>(vert));

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
	vector<pair<int, int>> dist(n, make_pair(INF, 0));
	queue<pair<int, int>> path;

	int num_vert = -1;

	dist[0].first = 0;
	int path_len = 0;

	for (int cur_iter = 0; cur_iter < n; ++cur_iter)
	{
		int min_dist = INF;
		int row = -1;
		for (int cur_num_vert = 0; cur_num_vert < n; ++cur_num_vert)
		{
			//We are looking for a minimal edge for unused vertices
			if (!used[cur_num_vert] && (dist[cur_num_vert].first < min_dist))
			{
				min_dist = dist[cur_num_vert].first;
				row = cur_num_vert;
			}
		}

		if (row == -1)
		{
			cout << "Ostov" << endl;
			//Add edges to "matrix" from the "temp_matrix"
			int i = -1;

			while (!path.empty())
			{
				++i;
				int start_edge = path.front().first;
				int end_edge = path.front().second;

				path.pop();

				if (start_edge == end_edge)
				{
					continue;
				}

				if (temp_matrix[start_edge][end_edge] == INF)
					continue;

				result_matrix[start_edge][end_edge] = temp_matrix[start_edge][end_edge];
				result_matrix[end_edge][start_edge] = temp_matrix[end_edge][start_edge];

				//show path on the screen
				cout << start_edge << "->" << end_edge << '\t';
				if (!((i + 1) % 3))
				{
					cout << endl;
				}
			}

			for (int num_vert = 0; num_vert < n; ++num_vert)
			{
				if (dist[num_vert].first == INF)
				{
					row = num_vert;
					min_dist = dist[num_vert].first = 0;
					dist[num_vert].second = num_vert;
					break;
				}
			}
			cout << "The length is " << path_len << endl;
			path_len = 0;
			num_vert = -1;
			min_dist = matrix[row][row];
		}

		//Calculation of results
		path_len += min_dist;

		
			pair<int, int> new_pair;
			new_pair.first = dist[row].second;
			new_pair.second = row;
			path.push(new_pair);
		
		++num_vert;

		used[row] = true;
		//Recalculate the minimum distances to the new used vert
		for (int col = 0; col < n; ++col)
		{
			if (!used[col] && temp_matrix[row][col] < dist[col].first)
			{
				dist[col].first = temp_matrix[row][col];
				dist[col].second = row;
			}
		}	
	}

	cout << "Ostov" << endl;
	//Add edges to "matrix" from the "temp_matrix"
	int i = -1;

	while (!path.empty())
	{
		++i;
		int start_edge = path.front().first;
		int end_edge = path.front().second;

		path.pop();

		if (temp_matrix[start_edge][end_edge] == INF)
			continue;

		result_matrix[start_edge][end_edge] = temp_matrix[start_edge][end_edge];
		result_matrix[end_edge][start_edge] = temp_matrix[end_edge][start_edge];

		//show path on the screen
		cout << start_edge << "->" << end_edge << '\t';
		if (!((i + 1) % 3))
		{
			cout << endl;
		}
	}

	cout << "The length is " << path_len << endl;

	show_external_matrix(&result_matrix);
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

	number_of_vertex = transform_num(number_of_vertex);
	if (number_of_vertex == ERROR)
	{
		return;
	}
	cout << "Degree = " << basic_graph::get_degree_of(number_of_vertex) << endl;
}

void normal_graph::show_degree_sequence()
{
	int size = user_vert.size();

	for (int i = 0; i < size; ++i)
	{
		cout << user_vert[i].num << ") ";
		show_degree_of(user_vert[i].num);
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

	from = transform_num(from);
	to = transform_num(to);
	if ((from == ERROR) || (to == ERROR))
	{
		return;
	}

	if (basic_graph::matrix.at(from).at(to) == 0 || basic_graph::matrix.at(to).at(from) == 0)
	{
		cout << "Error" << endl;
		return;
	}

	//if loop
	if (from == to)
	{
		matrix.at(from).at(to) -= 2;

		int num = get_unused_user_num();
		add_vertex(num);
		num = transform_num(num);

		matrix.at(from).at(num) += 1;
		matrix.at(num).at(from) += 1;
		return;
	}

	int num = get_unused_user_num();
	add_vertex(num);
	num = transform_num(num);

	//delete edge
	basic_graph::matrix.at(from).at(to) -= 1;
	basic_graph::matrix.at(to).at(from) -= 1;

	//connect new edge with vertex from
	basic_graph::matrix.at(from).at(num) += 1;
	basic_graph::matrix.at(num).at(from) += 1;

	//connect new edge with vertex to
	basic_graph::matrix.at(to).at(num) += 1;
	basic_graph::matrix.at(num).at(to) += 1;
}

vector<vector<int>>* normal_graph::get_matrix()
{
	return &(basic_graph::matrix);
}