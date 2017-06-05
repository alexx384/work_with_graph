#include "basic_graph.h"

using namespace std;

void basic_graph::adjacency_matrix(std::ifstream *work_file, std::vector<std::vector <int>> *matrix)
{
	//========== Open file ==========

	int len;
	char c = 0;
	for (len = 0; (c != '\n') && (!(*work_file).eof()); )
	{
		c = (*work_file).get();
		if (c == ' ')	++len;
	}
	++len;

	//========== Get lenght of row and column ==========

	(*matrix).resize(len, vector<int>(len));
	(*matrix).shrink_to_fit();

	(*work_file).seekg(ios_base::beg);

	char buff[3] = { 0 };
	for (int row = 0; row < len; ++row)
	{
		for (int col = 0; col < len; ++col)
		{
			(*work_file) >> (*matrix).at(row).at(col);
		}
	}
}

void basic_graph::incidence_matrix(std::ifstream *work_file, std::vector<std::vector <int>> *matrix)
{
	int count_col = 0;
	int count_row = 0;
	int flag = 1;
	char c;
	while (!(*work_file).eof())
	{
		c = (*work_file).get();
		if (flag && (c == ' '))	++count_col;
		if (c == '\n')
		{
			++count_row;
			flag = 0;
		}
	}
	++count_col;
	++count_row;

	//========== Get count of columns and rows from file ==========

	(*work_file).clear();
	(*work_file).seekg(ios_base::beg);

	(*matrix).resize(count_row, vector<int>(count_row));

	vector<vector <int>> temp_matrix(count_row, vector <int>(count_col));

	//========== Allocate two matrix. The temp_matrix, we will delete soon ==========

	for (int row = 0; row < count_row; ++row)
	{
		//char buff[3] = { 0 };
		for (int col = 0; col < count_col; ++col)
		{
			//(*work_file) >> buff;
			 //= atoi(buff);
			 (*work_file) >> temp_matrix.at(row).at(col);
		}
	}

	//========== Write matrix from file to temp_matrix

	for (int i = 0, col, row; i < count_col; ++i)
	{
		col = row = -1;
		for (int g = 0; g < count_row; ++g)
		{
			switch (temp_matrix.at(g).at(i))
			{
			case -1: col = g; break;
			case  1:
			{
				if (row == -1)	row = g;
				else
				{
					col = g;
					(*matrix).at(col).at(row) += 1;
				}

			}break;
			case  2:
			{
				col = row = g;
				(*matrix).at(row).at(col) += 1;
			}break;
			}
		}
		if ((col != -1) && (row != -1))
			(*matrix).at(row).at(col) += 1;
	}

	//========== Write adjacency matrix ==========

	temp_matrix.clear();
}

/*Transform num from user space to program space*/
int basic_graph::transform_num(int num)
{
	int size = user_vert.size();
	for (int i = 0; i < size; i++)
	{
		if (user_vert[i].num == num)
		{
			return user_vert[i].cur_num;
		}
	}
	cout << "Error unable to found vertex with num" << num << endl;
	return ERROR;
}

/*Transform num from program space to user space*/
int basic_graph::untransform_num(int cur_num)
{
	int size = user_vert.size();
	for (int i = 0; i < size; i++)
	{
		if (user_vert[i].cur_num == cur_num)
		{
			return user_vert[i].num;
		}
	}
	cout << "Error unable to found vertex with num" << cur_num << endl;
	return ERROR;
}

int basic_graph::add_user_num(int num, int matrix_num)
{
	int size = user_vert.size();
	for (int i = 0; i < size; i++)
	{
		if (user_vert[i].num == num)
		{
			cout << "Error: The vertice is already exist" << endl;
			return ERROR;
		}
	}
	
	num_vert temp_num;
	temp_num.num = num;
	temp_num.cur_num = matrix_num;
	user_vert.push_back(temp_num);

	return NO_ERROR;
}

int basic_graph::del_user_num(int num)
{
	int size = user_vert.size();
	for (int i = 0; i < size; i++)
	{
		if (user_vert[i].num == num)
		{
			int ret_num = user_vert[i].cur_num;
			user_vert.erase(user_vert.begin() + i);
			for (int k = i; k < size-1; k++)
			{
				user_vert[k].cur_num -= 1;
			}
			return ret_num;
		}
	}
	cout << "Error unable to found vertex with num" << num << endl;
	return ERROR;
}

int basic_graph::DFS_for_ford_fulkers(int node_num, int end_node, int cmin, int *used, 
	vector<vector<int>> *temp_matrix, int back_vert)
{
	if (node_num == end_node)	return cmin;

	used[node_num] = true;
	int delta = 0;
	for (int i = 0; i < matrix.at(node_num).size(); ++i)
	{
		if (((*temp_matrix)[node_num][i] > 0) && (node_num != i))
		{
			used[i] = false;
			int res_min = min(cmin, (*temp_matrix)[node_num][i]);
			delta += DFS_for_ford_fulkers(i, end_node, res_min, used,
				temp_matrix, node_num);

			if (delta > 0)
			{
				(*temp_matrix)[node_num][i] += delta;
				if (back_vert != -1)
				{
					(*temp_matrix)[back_vert][node_num] -= delta;
				}
			}
		}
	}

	return min(cmin, delta);
}

void basic_graph::ford_fulkerson()
{
	int vert = get_count_vertex();

	int source = -1;
	int sink = -1;

	for (int row = 0; row < vert; ++row)
	{
		int temp = 0;
		for (int col = 0; col < vert; ++col)
		{
			temp += matrix[col][row];
		}
		if (temp == 0)
		{
			source = row;
		}

		temp = 0;
		for (int col = 0; col < vert; ++col)
		{
			temp += matrix[row][col];
		}
		if (temp == 0)
		{
			sink = row;
		}
	}

	if ((source == -1) || (sink == -1))
	{
		cout << "error" << endl;
		return;
	}

	cout << "source " << untransform_num(source) << endl;
	cout << "sink " << untransform_num(sink) << endl;

	int cmin = 0;
	for (int i = 0; i < vert; ++i)
	{
		cmin += matrix[source][i];
	}

	vector<vector<int>> temp_matrix(vert, vector<int>(vert));
	int *used = new int[vert];
	for (int row = 0; row < vert; ++row)
	{
		used[row] = false;
		for (int col = 0; col < vert; ++col)
		{
			temp_matrix[row][col] = matrix[row][col];
		}
	}
	
	cout << DFS_for_ford_fulkers(source, sink, cmin, used, &temp_matrix, -1) << endl;
	delete[] used;

	cout << endl;

	for (int row = 0; row < vert; ++row)
	{
		for (int col = 0; col < vert; ++col)
		{
			cout << temp_matrix[row][col] << '/' << matrix[row][col] << ' ';
		}
		cout << endl;
	}
}

int basic_graph::get_unused_user_num()
{
	int size = user_vert.size();

	sort(user_vert.begin(), user_vert.end(), [](const num_vert &a, const num_vert &b)
	{
		return a.num < b.num;
	});

	int prev = 1;
	for (int i = 0; i < size; ++i, ++prev)
	{
		if ((user_vert[i].num-prev) > 0)
		{
			return prev;
		}
	}

	return prev;
}

deque<basic_graph::edge_struct> * basic_graph::get_edges()
{
	int vert = get_count_vertex();
	vector< vector <int>> temp_matrix(vert, vector<int>(vert));

	auto cmp = [](edge_struct left, edge_struct right)
	{
		return left.weight < right.weight;
	};
	//priority_queue<edge_struct, vector<edge_struct>, decltype(cmp)> q_prior(cmp);

	deque<edge_struct> *q_prior = new deque<edge_struct>;

	for (int col = 0; col < vert; ++col)
	{
		for (int row = 0; row < vert; ++row)
		{
			if (temp_matrix.at(row).at(col) == 0)
			{
				if (matrix.at(row).at(col) != 0)
				{
					edge_struct new_edge;
					new_edge.first = row;
					new_edge.second = col;
					new_edge.weight = matrix.at(row).at(col);
					q_prior->push_front(new_edge);
				}
				temp_matrix.at(row).at(col) = 1;
				if (!is_oriented)
				{
					temp_matrix.at(col).at(row) = 1;
				}
			}
		}
	}
	temp_matrix.clear();

	sort(q_prior->begin(), q_prior->end(), cmp);

	/*while (!q_prior.empty())
	{
		cout << q_prior.back().weight << endl;
		q_prior.pop_back();
	}*/

	return q_prior;
}

int basic_graph::DFS_for_kruskal(int node_num, int *used, int search_edge,
	vector<vector<int>> *temp_matrix)
{
	if (node_num == search_edge)	return ERROR;
	
	if (used[node_num] != 0)		return NO_ERROR;

	used[node_num] = 1;
	
	int ret_val = NO_ERROR;

	for (int i = 0; i < temp_matrix->at(node_num).size(); ++i)
	{
		if ((temp_matrix->at(node_num).at(i) != 0) && (node_num != i) && (used[i] == 0))
			ret_val += DFS_for_kruskal(i, used, search_edge, temp_matrix);
	}
	return ret_val;
}

void basic_graph::kruskal_algorithm()
{
	deque<edge_struct> *q_edge = get_edges();

	int vert = get_count_vertex();
	int *used = new int[vert];
	int *tmp_used = new int[vert];
	memset(used, 0, vert * sizeof(int));

	vector<vector<int>> temp_matrix(vert, vector<int>(vert));

	int i = 0;
	for(edge_struct cur_edge: *q_edge)
	{
		int start_edge = cur_edge.first;
		int end_edge = cur_edge.second;
		int weight = cur_edge.weight;

		//Reapair on cycle
		if (!used[start_edge] && !used[end_edge])
		{
			memset(tmp_used, 0, vert * sizeof(int));

			//The cycle may happen. So reapair on it 
			//IF "NO_ERROR" the cycle doesn't exist so we can the new vertex
			if (DFS_for_kruskal(start_edge, tmp_used, end_edge, &temp_matrix) == NO_ERROR)
			{
				temp_matrix[start_edge][end_edge] = weight;
				if (!is_oriented)
				{
					temp_matrix[end_edge][start_edge] = weight;
				}
			}
		} else {
			//Cycle doesn't exist so add the new edge
			temp_matrix[start_edge][end_edge] = weight;
			if (!is_oriented)
			{
				temp_matrix[end_edge][start_edge] = weight;
			}
		}
		++i;
	}
	
	//show result matrix. The main matrix is doesn't change
	show_external_matrix(&temp_matrix);

	delete q_edge;
	delete[] used;
	delete[] tmp_used;
}

int basic_graph::get_count_vertex()
{
	return matrix.size();
}

int basic_graph::get_count_edge()
{
	int edge = 0;
	int count = matrix.size();
	vector< vector <int>> temp_matrix(count, vector<int>(count));

	for (int col = 0; col < count; ++col)
	{
		for (int row = 0; row < count; ++row)
		{
			if (temp_matrix.at(row).at(col) == 0)
			{
				if (matrix.at(row).at(col) != 0)
				{
					int proc_val = 0;
					
					while (matrix.at(row).at(col) != proc_val)
					{
						proc_val += 1;
						++edge;
					}
					if ((row == col) && !is_oriented)
					{
						proc_val /= 2;
						edge -= proc_val;
					}
				}
				temp_matrix.at(row).at(col) = 1;
				if (!is_oriented)
				{
					temp_matrix.at(col).at(row) = 1;
				}
			}
		}
	}
	temp_matrix.clear();

	return edge;
}

int basic_graph::get_degree_of(int number_of_vertex)
{
	int degree = 0;
	int size = matrix.size();

	for (int i = 0; i < size; ++i)
	{
		degree += matrix.at(number_of_vertex).at(i);
	}

	return degree;
}

int basic_graph::get_distance(int from, int to)
{
	//--from;
	const vector<int> *distance = get_distance_to_all_vertex(from);
	//--to;
	int result = distance->at(to);

	delete distance;

	return result;
}

int basic_graph::get_eccentricity_of(int numbet_of_vertex)
{
	vector<int> *distance = get_distance_to_all_vertex(numbet_of_vertex);

	int result = *max_element(distance->begin(), distance->end());

	delete distance;

	return result;
}

vector<int> * basic_graph::get_distance_to_all_vertex(int number_of_vertex)
{
	int vert = get_count_vertex();
	vector<int> *dist = new vector<int>(vert);

	set<pair<int, int> > queue;

	vector< vector< pair<int, int>>> temp_matrix(vert);
	//where the pair(number_of_vertex, size)

	for (int row = 0; row < vert; ++row)
	{
		for (int col = 0; col < vert; ++col)
		{
			if (matrix.at(row).at(col))
			{
				pair<int, int> new_pair;
				new_pair.first = col;
				new_pair.second = 1;
				temp_matrix.at(row).push_back(new_pair);
			}
		}
	}


	dist->assign(vert, INT_MAX);

	dist->at(number_of_vertex) = 0;

	//Insert in sorted queue the pair(size, number_of vertex)
	for (int i = 0; i < vert; ++i)
	{
		queue.insert(make_pair(dist->at(i), i));
	}

	while (!queue.empty())
	{
		pair<int, int> cur = *queue.begin();
		queue.erase(queue.begin());

		for (int i = 0; i < (int)temp_matrix.at(cur.second).size(); ++i)
		{
			if (dist->at(temp_matrix.at(cur.second).at(i).first) > cur.first + temp_matrix.at(cur.second).at(i).second)
			{
				//Delete from queue pair(size, number of vertex)
				queue.erase(make_pair(dist->at(temp_matrix.at(cur.second).at(i).first), temp_matrix.at(cur.second).at(i).first));
				//Initialize dist with new value of distance
				dist->at(temp_matrix.at(cur.second).at(i).first) = cur.first + temp_matrix.at(cur.second).at(i).second;
				//Insert in the queue the new data about the same vertex
				queue.insert(make_pair(dist->at(temp_matrix.at(cur.second).at(i).first), temp_matrix.at(cur.second).at(i).first));
			}
		}
	}

	queue.clear();
	temp_matrix.clear();

	for (int i = 0; i < dist->size(); ++i)
	{
		if ((dist->at(i) < 0) || (dist->at(i) > INF))
			dist->at(i) = 0;
	}

	return dist;
}

std::vector<std::vector<int>>* basic_graph::get_count_of_connected_component()
{
	int vert = get_count_vertex();
	vector<vector<int>> *connected_component = new vector<vector<int>>;

	vector<int> check_matrix(vert);

	for (int i = 0; i < vert; i++)
	{
		if (check_matrix.at(i) == 0)
		{
			vector<int> exst_cmpnt;
			check_matrix.at(i) = 1;
			exst_cmpnt.push_back(i);
			const vector<int> *distance = get_distance_to_all_vertex(i);
			
			for (int cnt = 0; cnt < vert; cnt++)
			{
				if ((check_matrix.at(cnt) == 0) && (distance->at(cnt) > 0) && (distance->at(cnt) < INF))
				{
					exst_cmpnt.push_back(cnt);
					check_matrix.at(cnt) = 1;
				}
			}
			connected_component->push_back(exst_cmpnt);
			delete distance;
		}
	}
	
	return connected_component;
}

void basic_graph::show_count_vertex()
{
	cout << "Vertex = " << get_count_vertex() << endl;
}

void basic_graph::show_count_edge()
{
	cout << "Edges = " << get_count_edge() << endl;
}

void basic_graph::show_adjacency_matrix()
{
	int len = matrix.at(0).size();
	cout << "   ";
	for (int i = 0; i < len; ++i)
	{
		cout << untransform_num(i) << ' ';
	}
	cout << endl;
	cout << "===";
	for (int i = 0; i < len; ++i)
	{
		cout << "==";
	}
	cout << endl;
	for (int row = 0; row < len; ++row)
	{
		int utrans = untransform_num(row);
		if (utrans < 10)	cout << ' ';
		cout << utrans << '|';
		for (int col = 0; col < len; ++col)
		{
			cout << ' ' << matrix.at(row).at(col);
		}
		cout << endl;
	}
}

void basic_graph::show_incidence_matrix()
{
	int vert = get_count_vertex();
	int edge = get_count_edge();

	vector< vector<int>> temp_adj_matrix(vert, vector<int>(vert));
	vector< vector<int>> temp_inc_matrix(vert, vector<int>(edge));

	edge = 0;

	for (int row = 0; row < vert; ++row)
	{
		for (int col = 0; col < vert; ++col)
		{
			if (temp_adj_matrix.at(row).at(col) == 0)
			{
				temp_adj_matrix.at(row).at(col) = 1;
				if (matrix.at(row).at(col) != 0)
				{	
					int proc_val = 0;
					if (!is_oriented)
					{
						if (row != col)
						{
							while (matrix.at(row).at(col) != proc_val)
							{
								temp_inc_matrix.at(col).at(edge) = 1;

								temp_inc_matrix.at(row).at(edge) = 1;

								++edge;
								proc_val += 1;
							}
						} else {
							while (matrix.at(row).at(col) != proc_val)
							{
								proc_val += 2;
								temp_inc_matrix.at(row).at(edge) = 2;
								++edge;
							}
						}
					} else {
						if (row != col)
						{
							while (matrix.at(row).at(col) != proc_val)
							{
								temp_inc_matrix.at(col).at(edge) = -1;

								temp_inc_matrix.at(row).at(edge) = 1;

								++edge;
								proc_val += 1;
							}
						} else
						{
							while (matrix.at(row).at(col) != proc_val)
							{
								proc_val += 1;
								temp_inc_matrix.at(row).at(edge) = 1;
								++edge;
							}
						}
					}
					temp_adj_matrix.at(col).at(row) = 1;
				}
			}
		}
	}

	cout << right;
	for (int row = 0; row < vert; ++row)
	{
		cout << setw(3) << user_vert[row].num << '|';
		for (int col = 0; col < edge; ++col)
		{
			cout << setw(3) << temp_inc_matrix.at(row).at(col);
		}
		cout << endl;
	}

	temp_adj_matrix.clear();
	temp_inc_matrix.clear();
}

void basic_graph::show_adjacency_list()
{
	int vert = get_count_vertex();

	for (int row = 0; row < vert; ++row)
	{
		cout << " {" << user_vert[row].num;
		for (int col = 0; col < vert; ++col)
		{
			if (!is_oriented && (row == col))
			{
				for (int i = 0; i < matrix.at(row).at(col); i+=2)
				{
					cout << ' ' << untransform_num(col);
				}
			} else {
				for (int i = 0; i < matrix.at(row).at(col); ++i)
				{
					cout << ' ' << untransform_num(col);
				}
			}
		}
		cout << '}' << endl;
	}
}

void basic_graph::show_pendant_vertex()
{
	int vert = get_count_vertex();

	for (int row = 0; row < vert; ++row)
	{
		if (get_degree_of(row) == 1)
			cout << ' ' << row + 1;
	}
}

void basic_graph::show_isolated_vertex()
{
	int vert = get_count_vertex();

	for (int row = 0; row < vert; ++row)
	{
		if (get_degree_of(row) == 0)
			cout << ' ' << row + 1;
	}
}

void basic_graph::show_distance(int from, int to)
{
	int vert = get_count_vertex();

	if (from == to) { cout << "Error. 'From' and 'to' need to be different" << endl; return; }

	from = transform_num(from);
	to = transform_num(to);
	if ((from == ERROR) || (to == ERROR))
	{
		return;
	}

	int distance = get_distance(from, to);

	if (distance == 0) { cout << "Error, the value in a different connected component" << endl; return; }

	cout << "The distance is " << distance << endl;
}

void basic_graph::show_eccentricity_of(int number_of_vertex)
{
	number_of_vertex = transform_num(number_of_vertex);
	if (number_of_vertex == ERROR)
	{
		return;
	}
	cout << "The eccentricity is" << get_eccentricity_of(number_of_vertex) << endl;
}

void basic_graph::show_length_of_radius()
{
	vector<vector<int>> *radius = get_length_of_radius();

	for (int i = 0; i < radius->size(); i++)
	{
		cout << "The radius is " << radius->at(i).back() << endl;
	}
	cout << "Warning: the value might be incorrect, because we couldn't " << endl;
	cout << "take into account all the connectivity components" << endl;
	delete radius;
}

void basic_graph::show_length_of_diameter()
{
	vector<vector<int>> *diameter = get_length_of_diameter();

	for (int i = 0; i < diameter->size(); i++)
	{
		cout << "The diameter is " << diameter->at(i).back() << endl;
	}
	delete diameter;
	//cout << "The diameter is " << floyd_alg() << endl;
	cout << "Warning: the value might be incorrect, because we couldn't " << endl;
	cout << "take into account all the connectivity components" << endl;
}

void basic_graph::show_center_number()
{
	vector<vector<int>> *radius = get_length_of_radius();

	for (int size = 0; size < radius->size(); size++)
	{
		int len_radius = radius->at(size).back();
		radius->at(size).pop_back();
		for (int i = 0; i < radius->at(size).size(); ++i)
		{
			if (get_eccentricity_of(radius->at(size).at(i)) == len_radius)
				cout << "center is " << untransform_num(radius->at(size).at(i)) << endl;
		}
	}
	delete radius;
}

void basic_graph::show_peripheral_number()
{
	vector<vector<int>> *diameter = get_length_of_diameter();

	for (int size = 0; size < diameter->size(); size++)
	{
		int len_diam = diameter->at(size).back();
		diameter->at(size).pop_back();
		for (int i = 0; i < diameter->at(size).size(); ++i)
		{
			if (get_eccentricity_of(diameter->at(size).at(i)) == len_diam)
				cout << "peripheral is " << untransform_num(diameter->at(size).at(i)) << endl;
		}
	}
	delete diameter;
}

void basic_graph::add_vertex(int number_of_vertex)
{
	int vert = get_count_vertex();
	if (add_user_num(number_of_vertex, vert) == ERROR)
	{
		return;
	}
	++vert;

	for (int i = 0; i < vert-1; ++i)
	{
		matrix.at(i).resize(vert);
	}
	matrix.resize(vert, vector<int>(vert));
}

void basic_graph::delete_vertex(int number_of_vertex)
{
	int vert = get_count_vertex();

	number_of_vertex = del_user_num(number_of_vertex);
	if (number_of_vertex == ERROR)
	{
		return;
	}

	for (int i = 0; i < vert; ++i)
	{
		matrix.at(i).erase(matrix[i].begin()+number_of_vertex);
	}
	matrix.erase(matrix.begin()+number_of_vertex);
}

void basic_graph::add_edge(int from, int to)
{
	int vert = get_count_vertex();

	from = transform_num(from);
	to = transform_num(to);

	if ((from == ERROR) || (to == ERROR))
	{
		return;
	}

	matrix.at(from).at(to) += 1;
}

void basic_graph::delete_edge(int from, int to)
{
	int vert = get_count_vertex();

	from = transform_num(from);
	to = transform_num(to);

	if ((from == ERROR) || (to == ERROR))
	{
		return;
	}

	if (matrix.at(from).at(to) != 0)
	{
		matrix.at(from).at(to) -= 1;
	} else {
		cout << "The edge is doesn't exist" << endl;
	}
}

std::vector<std::vector<int>> * basic_graph::get_length_of_diameter()
{
	vector<vector<int>> *connected_component = get_count_of_connected_component();

	int size;

	vector<vector<int>> *diameter = new vector<vector<int>>;

	for (int component = 0; component < connected_component->size(); component++)
	{
		int length = 0;
		vector<int> cur_diam;
		for (int i = 0; i < connected_component->at(component).size(); ++i)
		{
			cur_diam.push_back(connected_component->at(component).at(i));

			size = get_eccentricity_of(connected_component->at(component).at(i));
			if (size > INF)
			{
				size = 0;
			}
			if (length < size)		length = size;
		}
		cur_diam.push_back(length);
		diameter->push_back(cur_diam);
	}

	delete connected_component;

/*===== return value =====
| num_of_vert || num_of_vert | ... | length_of_diameter |
*/
	return diameter;
}

std::vector<std::vector<int>> * basic_graph::get_length_of_radius()
{
	vector<vector<int>> *connected_component = get_count_of_connected_component();

	int size;

	vector<vector<int>> *radius = new vector<vector<int>>;

	for (int component = 0; component < connected_component->size(); component++)
	{
		int length = get_eccentricity_of(connected_component->at(component).at(0));
		vector<int> cur_diam;
		for (int i = 0; i < connected_component->at(component).size(); ++i)
		{
			cur_diam.push_back(connected_component->at(component).at(i));

			size = get_eccentricity_of(connected_component->at(component).at(i));

			if (length > size)		length = size;
		}
		cur_diam.push_back(length);
		radius->push_back(cur_diam);
	}

	delete connected_component;

	/*===== return value =====
	| num_of_vert || num_of_vert | ... | length_of_diameter |
	*/
	return radius;
}

void basic_graph::insert_matrix_to(int row, int col, std::vector<std::vector<int>>* external_matrix)
{
	int vert = external_matrix->size();

	for (int local_row = row, external_row = 0; local_row < vert + row; ++local_row, ++external_row)
	{
		for (int local_col = col, external_col = 0; local_col < vert + col; ++local_col, ++external_col)
		{
			matrix.at(local_row).at(local_col) = external_matrix->at(external_row).at(external_col);
		}
	}
}

int basic_graph::get_count_edges_in(std::vector<std::vector<int>>* external_matrix)
{
	int vert = external_matrix->size();

	vector<pair<int, int>>	arr_edges;
	arr_edges.clear();

	for (int row = 0; row < vert; ++row)
	{
		for (int col = 0; col < vert; ++col)
		{
			if (external_matrix->at(row).at(col) != 0)
			{
				pair<int, int> temp_pair;
				if (row > col) {
					temp_pair.first = col;
					temp_pair.second = row;
				}
				else {
					temp_pair.first = row;
					temp_pair.second = col;
				}

				int size = arr_edges.size();
				for (int i = 0; i < size; ++i)
				{
					if (arr_edges.at(i).first == temp_pair.first
						&& arr_edges.at(i).second == temp_pair.second)	size = -1;
				}

				if (size != -1) {
					arr_edges.push_back(temp_pair);
				}
			}
		}
	}

	int size = arr_edges.size();
	arr_edges.clear();

	return size;
}

void basic_graph::show_external_matrix(std::vector<std::vector<int>>* external_matrix)
{
	int len = external_matrix->at(0).size();
	cout << "   ";
	for (int i = 0; i < len; ++i)
	{
		cout << i + 1 << ' ';
	}
	cout << endl;
	cout << "===";
	for (int i = 0; i < len; ++i)
	{
		cout << "==";
	}
	cout << endl;
	for (int row = 0; row < len; ++row)
	{
		cout << row + 1 << '|';
		for (int col = 0; col < len; ++col)
		{
			if (external_matrix->at(row).at(col) == INF)
			{
				cout << ' ' << '*';
			} else {
				cout << ' ' << external_matrix->at(row).at(col);
			}
		}
		cout << endl;
	}
}

void basic_graph::make_graph_addition()
{
	int vert = get_count_vertex();

	for (int row = 0; row < vert; ++row)
	{
		for (int col = 0; col < vert; ++col)
		{
			if (row == col)
			{

				if (matrix.at(row).at(col) >= 2) 
				{
					cout << "We couldn't build that graph, because there are loops or multiple edges" << endl;
					return;
				}
				matrix.at(row).at(col) = 0;
				continue;
				//continue;
			}
			if (matrix.at(row).at(col) != 0)	matrix.at(row).at(col) = 0;
			else								matrix.at(row).at(col) = 1;
		}
	}
}

void basic_graph::make_graph_contraction(std::vector<int> *user_arr)
{
	int vert = get_count_vertex();

	vector<int>	temp_vertex_col(vert);
	vector<int>	temp_vertex_raw(vert);

	for (int i = 0; i < user_arr->size(); ++i)
	{
		int temp = user_arr->at(i);
		temp = transform_num(temp);
		if (temp == ERROR)
		{
			return;
		}
		user_arr->at(i) = temp;
		/*===== delete loop =====*/
		matrix.at(temp).at(temp) = 0;
	}

 /*wirite edges of first edge and others*/
	int user_size = user_arr->size();
	for (int i = 0; i < user_size; ++i)
	{
		int first = user_arr->at(i);
		for (int col = 0; col < vert; ++col)
		{
			if(temp_vertex_raw[col] == 0)
				temp_vertex_raw[col] = matrix[first][col];
		}
	}


	for (int i = 0; i < user_size; ++i)
	{
		int first = user_arr->at(i);
		for (int row = 0; row < vert; ++row)
		{
			if (temp_vertex_col[row] == 0)
				temp_vertex_col[row] = matrix[row][first];
		}
	}

	int frst_temp = user_arr->at(0);
	for (int i = 0; i < vert; ++i)
	{
		if (frst_temp != i)
		{
			matrix[frst_temp][i] = temp_vertex_raw[i];
		}
	}
	for (int i = 0; i < vert; ++i)
	{
		if (frst_temp != i)
		{
			matrix[i][frst_temp] = temp_vertex_col[i];
		}
	}


	/*from = transform_num(from);
	to = transform_num(to);
	if ((from == ERROR) || (to == ERROR))
	{
		return;
	}*/

	//translate to user num
	for (int i = 1; i < user_size; ++i)
	{
		int temp = user_arr->at(i);
		temp = untransform_num(temp);
		user_arr->at(i) = temp;
	}
	for (int i = 1; i < user_size; ++i)
	{
		int temp = user_arr->at(i);
		delete_vertex(temp);
	}
}

void basic_graph::make_vertex_identification(int first, int second)
{
	int vert = get_count_vertex();

	first = transform_num(first);
	second = transform_num(second);
	if ((first == ERROR) || (second == ERROR))
	{
		return;
	}

	if (first == second)
	{
		cout << "Warning: Nothing happend" << endl;
		return;
	}

	for (int i = 0; i < vert; ++i)
	{
		if ((matrix.at(first).at(i) == 0) && (first != i))
		{
			matrix.at(first).at(i) += matrix.at(second).at(i);
		}
	}

	for (int i = 0; i < vert; ++i)
	{
		if ((matrix.at(i).at(first) == 0) && (first != i))
		{
			matrix.at(i).at(first) += matrix.at(i).at(second);
		}
	}

	second = untransform_num(second);
	delete_vertex(second);
}

void basic_graph::make_vertex_dublicate(int number_of_vertex)
{
	int vert = get_count_vertex();

	number_of_vertex = transform_num(number_of_vertex);

	if (number_of_vertex == ERROR)
	{
		return;
	}
	
	int num = get_unused_user_num();
	add_vertex(num);
	num = transform_num(num);

	for (int i = 0; i < vert; ++i)
	{
		if (number_of_vertex != i)
		{
			matrix.at(num).at(i) = matrix.at(number_of_vertex).at(i);
		}
	}

	for (int i = 0; i < vert; ++i)
	{
		if (number_of_vertex != i)
		{
			matrix.at(i).at(num) += matrix.at(i).at(number_of_vertex);
		}
	}
}

void basic_graph::make_proliferation_with(int number_of_vertex)
{
	int vert = get_count_vertex();

	make_vertex_dublicate(number_of_vertex);
	number_of_vertex = transform_num(number_of_vertex);

	basic_graph::matrix.at(number_of_vertex).at(vert) += 1;
	basic_graph::matrix.at(vert).at(number_of_vertex) += 1;
}

void basic_graph::make_union_with_graph(vector<vector<int>>* external_matrix)
{
	int start_count_vert = get_count_vertex();
	int vert = external_matrix->size();
	vector<int> temp_matrix(vert);

	for (int i = 0; i < vert; ++i)
	{
		int num = get_unused_user_num();
		add_vertex(num);
		num = transform_num(num);
		temp_matrix[i] = num;
	}

	for (int local_row = 0; local_row < vert; ++local_row)
	{
		for (int local_col = 0; local_col < vert; ++local_col)
		{
			int row = temp_matrix[local_row];
			int col = temp_matrix[local_col];
			matrix.at(row).at(col) = external_matrix->at(local_row).at(local_col);
		}
	}
}

void basic_graph::make_connection_with_graph(std::vector<std::vector<int>>* external_matrix)
{
	int start_vert = get_count_vertex();
	make_union_with_graph(external_matrix);
	int vert = get_count_vertex();

	for (int row = 0; row < start_vert; ++row)
	{
		for (int col = start_vert; col < vert; ++col)
		{
			matrix.at(row).at(col) = 1;
		}
	}

	for (int row = start_vert; row < vert; ++row)
	{
		for (int col = 0; col < start_vert; ++col)
		{
			matrix.at(row).at(col) = 1;
		}
	}
}

void basic_graph::make_graph_product_with(std::vector<std::vector<int>>* external_matrix)
{
	int start_vert = get_count_vertex();

	int vert = external_matrix->size();

	vector<vector<int>> temp_original_marix(start_vert, vector<int>(start_vert));
	vector<vector<int>> temp_matrix(start_vert, vector<int>(start_vert));

	temp_original_marix = matrix;

	for (int i = 1; i < vert; ++i)
	{
		make_union_with_graph(&temp_original_marix);
	}

	//Fill temp matrix such as -------------------------------------->	1 0 0
	for (int row = 0; row < start_vert; ++row)						//  0 1 0
	{																//  0 0 1
		for (int col = 0; col < start_vert; col++)
		{
			if (row == col)	temp_matrix.at(row).at(col) = 1;
		}
	}

	//insert temp_matrix

	for (int row = 0; row < vert; ++row)
	{
		for (int col = 0; col < vert; ++col)
		{
			if ((col != row) && (external_matrix->at(row).at(col) != 0))
			{
				insert_matrix_to(row*start_vert, col*start_vert, &temp_matrix);
			}
		}
	}
}

void basic_graph::algorithm_BFS(int node_num)
{
	int vert = get_count_vertex();

	if ((node_num < 1) || (node_num > vert))
	{
		cout << "Error: The value is out of range" << endl;
		return;
	}

	vector<int> used(vert);
	//vector<int> path(vert);
	vector<vector<int>> temp_matrix(vert, vector<int>(vert));

	int num_path = 1;

	node_num = transform_num(node_num);

	BFS(node_num, &temp_matrix, &used);
	cout << endl;

	show_external_matrix(&temp_matrix);
}

void basic_graph::BFS(int node_num, vector<vector<int>> *temp_matrix,
	vector<int> *used)
{
	int vert = get_count_vertex();

	queue<int> turn;

//	vector<int> path(vert);
	int num_path = 1;

	//path[0] = node_num;
	turn.push(node_num);
	(*used).at(node_num) = 1;

	while (!turn.empty()) 
	{
		int ind = turn.front();
		turn.pop();
		//cout << (ind+1) << ' ';
		for (int i = 0; i < vert; ++i)
		{
			if ((matrix[ind][i] != 0) && (ind != i) && ((*used).at(i) == 0))
			{
				(*temp_matrix)[ind][i] = matrix[ind][i];
				cout << ind << "->" << i << endl;
			//	(*temp_matrix)[i][ind] = matrix[ind][i];
				turn.push(i);
				//path[num_path] = i;
				++num_path;
				(*used).at(i) = 1;
			}
		}
	}

	for (int i = 0; i < vert; ++i)
	{
		if ((*used)[i] == 0)
			BFS(i, temp_matrix, used);
	}

	/*for (int i = 0; i < vert; ++i)
	{
		matrix[i].assign(vert, 0);
	}

	for (int i = 0; i < num_path-1; ++i)
	{
		if (!is_oriented)
		{
			matrix[path[i + 1]][path[i]] += 1;
		}
		matrix[path[i]][path[i + 1]] += 1;

		cout << path[i] + 1 << " -> ";
	}
	cout << path[num_path - 1] + 1 << endl;*/
}

void basic_graph::DFS(int node_num, int *used, queue<int> *path)
{
	if (used[node_num] != 0) return;
	
	path->push(node_num);
	used[node_num] = 1;
	cout << node_num + 1 << " -> ";
	//path[node_num] = node_from; 
//	if (v == finish) - if path was found 
	for (int i = 0; i < matrix.at(node_num).size(); ++i)  
	{
		if((matrix.at(node_num).at(i) != 0) && (node_num != i) && (used[i] == 0))
			DFS(i, used, path);  
	}
}

void basic_graph::DFS_search(int node_start)
{
	int vert = get_count_vertex();

	if ((node_start < 1) || (node_start > vert))
	{
		cout << "Error: The value is out of range" << endl;
		return;
	}

	int *used = new int[vert];

	queue<int> path;

	for (int i = 0; i < vert; ++i)
	{
		used[i] = 0;
	}

	--node_start;
	basic_graph::DFS(node_start, used, &path);

	for (int i = 0; i < vert; ++i)
	{
		matrix[i].assign(vert, 0);
	}

	int start_num = path.front();
	path.pop();

	while (!path.empty())
	{
		int cur_num = path.front();
		path.pop();
		
		if (!is_oriented)
		{
			matrix[cur_num][start_num] += 1;
		}
		matrix[start_num][cur_num] += 1;
		start_num = cur_num;
	}

	delete used;
}

int basic_graph::floyd_alg(int start_n, int end_n)
{
	int vert = get_count_vertex();
	int max_val = 0;

	start_n = transform_num(start_n);
	end_n = transform_num(end_n);

	vector< vector< int>> temp_matrix(vert, vector<int>(vert));
	vector< vector< int>> parent_matrix(vert, vector<int>(vert));
	for (int row = 0; row  < vert; ++row)
	{
		for (int col = 0; col < vert; ++col)
		{
			if ((matrix.at(row).at(col) == 0) && (row != col))
			{
				temp_matrix.at(row).at(col) = INF;
			} else {
				temp_matrix.at(row).at(col) = matrix.at(row).at(col);
			}
			parent_matrix[row][col] = col;
		}
	}

	for (int k = 0; k < vert; ++k) {
		for (int i = 0; i < vert; ++i) {
			for (int j = 0; j < vert; ++j) {
				if (i != j && temp_matrix.at(i).at(k) != INF && temp_matrix.at(k).at(j) != INF) {
					if (temp_matrix.at(i).at(j) == INF) 
					{
						temp_matrix.at(i).at(j) = temp_matrix.at(i).at(k) + temp_matrix.at(k).at(j);
						parent_matrix[i][j] = parent_matrix[i][k];
					} else {
						if (temp_matrix.at(i).at(j) > temp_matrix.at(i).at(k) + temp_matrix.at(k).at(j))
						{
							temp_matrix.at(i).at(j) = temp_matrix.at(i).at(k) + temp_matrix.at(k).at(j);
							parent_matrix[i][j] = parent_matrix[i][k];
						}
					}
				}
			}
		}
	}

	show_external_matrix(&temp_matrix);
	cout << endl;
	show_external_matrix(&parent_matrix);

	if (parent_matrix[start_n][end_n] == end_n)
	{
		cout << "error the path is not found" << endl;
		return 0;
	}

	cout << temp_matrix[start_n][end_n] << endl;

	while (parent_matrix[start_n][end_n] != start_n)
	{
		cout << untransform_num(start_n) << "->";
		start_n = parent_matrix[start_n][end_n];
	}
	cout << untransform_num(end_n) << endl;
	//cout << max_val << endl;
	return max_val;
}

bool basic_graph::DFS_for_topological(int node_num, int *color, stack<int> *stack_vert)
{
	if (color[node_num] == 1) return true;
	if (color[node_num] == 2) return false;

	color[node_num] = 1;
	
	for (int i = 0; i < matrix.at(node_num).size(); ++i)
	{
		if ((matrix.at(node_num).at(i) != INF) && (node_num != i))
		{
			if (DFS_for_topological(i, color, stack_vert) == true)
				return true;
		}
	}

	stack_vert->push(node_num);
	color[node_num] = 2;
	return false;
}

void basic_graph::non_loop_search(int start_num, int end_num)
{
	int vert = get_count_vertex();

	start_num = transform_num(start_num);
	end_num = transform_num(end_num);
	if ((start_num == ERROR) && (end_num == ERROR))
	{
		return;
	}

	vector<num_vert> temp_user_vert(vert);
	for (int i = 0; i < vert; ++i)
	{
		temp_user_vert[i] = user_vert[i];
	}

	vector<vector<int>> *result_matrix = topological_sort(&start_num, &end_num);
	if (!result_matrix)
	{
		cout << "Error: something went wrong" << endl;
		return;
	}

	vector<vector<int>> temp_matrix(vert, vector<int>(vert));
	cout << "after topological sort" << endl;
	temp_matrix = *result_matrix;
	show_external_matrix(&temp_matrix);

	delete result_matrix;

	vector<int> dist(vert, INF);
	vector<int> path(vert);
	dist[start_num] = 0;
	for (int pos = start_num; pos < end_num; ++pos)
	{
		for (int w = pos; w < end_num; ++w)
		{
			if (dist[w+1] > dist[pos] + temp_matrix[pos][w+1])
			{
				dist[w + 1] = dist[pos] + temp_matrix[pos][w+1];
				path[w + 1] = pos;
			}
		}
	}

	cout << endl;
	
	for (int i = 0; i < vert; ++i)
	{
		cout << untransform_num(i) << ' ';
	}
	cout << endl;
	for (int i = 0; i < vert; ++i)
	{
		cout << dist[i] << ' ';
	}
	cout << endl;
	for (int i = 0; i < vert; ++i)
	{
		cout << untransform_num(path[i]) << ' ';
	}
	cout << endl;

	stack<int> true_path;

	int cur_num = end_num;
	true_path.push(end_num);

	if (dist[end_num] >= INF)
	{
		cout << "Error: The vertices in different connected component" << endl;
		user_vert = temp_user_vert;
		return;
	} else
	{
		cout << "the length is " << dist[end_num] << endl;
	}

	while (cur_num != start_num)
	{
		cur_num = path[cur_num];

		true_path.push(cur_num);
	}

	while (!true_path.empty())
	{
		cout << untransform_num(true_path.top()) << "->";
		true_path.pop();
	}
	
	user_vert = temp_user_vert;
}

vector<vector<int>> * basic_graph::topological_sort(int *start_node, int *end_node)
{
	int vert = get_count_vertex();
	bool Cycle;
	stack<int> stack_vert;
	int *color = new int[vert];

	for (int row = 0; row < vert; ++row)
	{
		for (int col = 0; col < vert; ++col)
		{
			if (matrix[row][col] == 0)
			{
				matrix[row][col] = INF;
			}
		}
	}


	for (int i = 0; i < vert; ++i)
	{
	    Cycle = DFS_for_topological(i, color, &stack_vert);
		if (Cycle)
		{
			delete[] color;
			return nullptr;
		}
	}
	delete[] color;

	int start_n = 0, end_n = 0;
	if (start_node)
	{
		start_n = *start_node;
	}
	if (start_node)
	{
		end_n = *end_node;
	}

	vector<int> temp_arr(vert);
	int done_temp_start = 0;
	int done_temp_end = 0;
	for (int i = 0; i < vert; ++i)
	{
		int temp = stack_vert.top();
		temp_arr[i] = temp;
		cout << i << ") " << temp << endl;

		if (temp == start_n && start_node && !done_temp_start)
		{
			*start_node = i;
			done_temp_start = 1;
		}

		if (temp == end_n && end_node && !done_temp_end)
		{
			*end_node = i;
			done_temp_end = 1;
		}

		user_vert[i].num = temp + 1;
		//user_vert[i].cur_num = temp;

		stack_vert.pop();
	}

	vector<vector<int>> *result_matrix = new vector<vector<int>>;
	result_matrix->resize(vert);
	for (int i = 0; i < vert; ++i)
	{
		(*result_matrix)[i].resize(vert);
	}

	vector<vector<int>> temp_matrix(vert, vector<int>(vert));
	for (int i = 0; i < vert; ++i)
	{
		for (int pos = 0; pos < vert; ++pos)
		{
			temp_matrix[i][pos] = matrix[temp_arr[i]][temp_arr[pos]];
		}
	}

	for (int row = 0; row < vert; ++row)
	{
		for (int col = 0; col < vert; ++col)
		{
			(*result_matrix)[row][col] = temp_matrix[row][col];
		}
	}

	show_external_matrix(result_matrix);
	return result_matrix;
}