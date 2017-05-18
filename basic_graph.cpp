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
			(*work_file) >> buff;
			(*matrix).at(row).at(col) = atoi(buff);
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
		char buff[3] = { 0 };
		for (int col = 0; col < count_col; ++col)
		{
			(*work_file) >> buff;
			temp_matrix.at(row).at(col) = atoi(buff);
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
				temp_matrix.at(col).at(row) = 1;
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
	--from;
	const vector<int> *distance = get_distance_to_all_vertex(from);
	--to;
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
		if ((dist->at(i) < 0) || (dist->at(i) > 2000000000))
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
				if ((check_matrix.at(cnt) == 0) && (distance->at(cnt) > 0) && (distance->at(cnt) < 2000000000))
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
		if (row + 1 < 10)	cout << ' ';
		cout << row + 1 << '|';
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
		cout << " {" << row + 1;
		for (int col = 0; col < vert; ++col)
		{
			if (!is_oriented && (row == col))
			{
				for (int i = 0; i < matrix.at(row).at(col); i+=2)
				{
					cout << ' ' << col + 1;
				}
			} else {
				for (int i = 0; i < matrix.at(row).at(col); ++i)
				{
					cout << ' ' << col + 1;
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
	if ((from > vert) || (to > vert) || (from < 1) || (to < 1))
	{ 
		cout << "Error, the value is out of range" << endl; 
		return; 
	}

	if (from == to) { cout << "Error. 'From' and 'to' need to be different" << endl; return; }

	int distance = get_distance(from, to);

	if (distance == 0) { cout << "Error, the value in a different connected component" << endl; return; }

	cout << "The distance is " << distance << endl;
}

void basic_graph::show_eccentricity_of(int number_of_vertex)
{
	cout << "The eccentricity is" << get_eccentricity_of(number_of_vertex - 1) << endl;
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
	/*vector<vector<int>> *diameter = get_length_of_diameter();

	for (int i = 0; i < diameter->size(); i++)
	{
		cout << "The diameter is " << diameter->at(i).back() << endl;
	}
	delete diameter;*/
	cout << "The diameter is " << floyd_alg() << endl;
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
				cout << "center is " << radius->at(size).at(i) + 1 << endl;
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
				cout << "peripheral is " << diameter->at(size).at(i) + 1 << endl;
		}
	}
	delete diameter;
}

void basic_graph::add_vertex()
{
	int vert = get_count_vertex();
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

	if (number_of_vertex < 1 || number_of_vertex > vert)
	{
		cout << "Error: unable find vertex" << endl;
		return;
	}

	--number_of_vertex;

	for (int i = 0; i < vert; ++i)
	{
		matrix.at(i).erase(matrix[i].begin()+number_of_vertex);
	}
	matrix.erase(matrix.begin()+number_of_vertex);
}

void basic_graph::add_edge(int from, int to)
{
	int vert = get_count_vertex();

	if ((from > vert) || (to > vert) || (from < 1) || (to < 1))
	{
		cout << "Error, the value is out of range" << endl;
		return;
	}

	--from;
	--to;

	matrix.at(from).at(to) += 1;
}

void basic_graph::delete_edge(int from, int to)
{
	int vert = get_count_vertex();

	if ((from > vert) || (to > vert) || (from < 1) || (to < 1))
	{
		cout << "Error, the value is out of range" << endl;
		return;
	}

	--from;
	--to;

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
			if (size > 2000000000)
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
			cout << ' ' << external_matrix->at(row).at(col);
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

void basic_graph::make_graph_contraction(int from, int to)
{
	int vert = get_count_vertex();

	vector<int>	temp_vertex_col;
	vector<int>	temp_vertex_raw;

	if ((from > vert) || (to > vert) || (from < 1) || (to < 1))
	{
		cout << "The value is out of range" << endl;
		return;
	}

	--from;
	--to;

/*===== if error ===== */
	if ((matrix.at(from).at(to) == 0) || (matrix.at(to).at(from) == 0))
	{
		cout << "Error: we haven't so vertex" << endl;
		return;
	}

	/*if (from == to)
	{
		if (!is_oriented)
		{
			matrix.at(from).at(to) -= 1;
		}
		matrix.at(from).at(to) -= 1;
		return;
	}*/

/*===== delete loop =====*/
	matrix.at(from).at(from) = 0;
	matrix.at(to).at(to) = 0;

//===== delete multiple edges =====
	for (int len = 0; len < vert; ++len)
	{
		if ((matrix.at(from).at(len) == matrix.at(to).at(len)) 
			&& (matrix.at(from).at(len) > 0) && (matrix.at(to).at(len) > 0))
		{	
			matrix.at(len).at(to) = 0;
			matrix.at(to).at(len) = 0;
		}
	}

/*===== write edges of first vertex =====*/
	for (int i = 0; i < vert; ++i)
	{
		if ((matrix.at(from).at(i) == 1) && (i != to))
		{
			if(i < from)	temp_vertex_col.push_back(i);
			else			temp_vertex_col.push_back(i-1);
		}
	}

/*===== write edges of second vertex =====*/
	for (int i = 0; i < vert; ++i)
	{
		if ((matrix.at(i).at(from) == 1) && (i != to))
		{
			if (i < from)	temp_vertex_raw.push_back(i);
			else			temp_vertex_raw.push_back(i - 1);
		}
	}

	if (to > from)	--to;

	delete_vertex(from+1);
	
	for (auto &element : temp_vertex_col)	basic_graph::matrix.at(to).at(element) += 1;
	
	for (auto &element : temp_vertex_raw)	basic_graph::matrix.at(element).at(to) += 1;
}

void basic_graph::make_vertex_identification(int first, int second)
{
	int vert = get_count_vertex();

	if ((first > vert) || (second > vert) || (first < 1) || (second < 1))
	{
		cout << "Error: we haven't so vertex" << endl;
		return;
	}

	--first;
	--second;

	if (first == second)
	{
		cout << "Warning: Nothing happend" << endl;
		return;
	}

	/*
	Убрать вверху условие
	if (first == second)
	{
		if (!is_oriented)
		{
			matrix.at(first).at(second) -= 1;
		}
		matrix.at(first).at(second) -= 1;
		return;
	}*/

	for (int i = 0; i < vert; ++i)
	{
		matrix.at(first).at(i) += matrix.at(second).at(i);
	}

	for (int i = 0; i < vert; ++i)
	{
		matrix.at(i).at(first) += matrix.at(i).at(second);
	}

	delete_vertex(second+1);
}

void basic_graph::make_vertex_dublicate(int number_of_vertex)
{
	int vert = get_count_vertex();
	if ((number_of_vertex > vert) || (number_of_vertex < 1))
	{
		cout << "Error: The value is out of range" << endl;
		return;
	}

	--number_of_vertex;
	add_vertex();

	for (int i = 0; i < vert; ++i)
	{
		if (number_of_vertex != i)
		{
			matrix.at(vert).at(i) = matrix.at(number_of_vertex).at(i);
		}
	}

	for (int i = 0; i < vert; ++i)
	{
		if (number_of_vertex != i)
		{
			matrix.at(i).at(vert) += matrix.at(i).at(number_of_vertex);
		}
	}
}

void basic_graph::make_proliferation_with(int number_of_vertex)
{
	int vert = get_count_vertex();

	if ((number_of_vertex > vert) || (number_of_vertex < 1))
	{
		cout << "Error: The value is out of range" << endl;
		return;
	}

	make_vertex_dublicate(number_of_vertex);
	--number_of_vertex;

	basic_graph::matrix.at(number_of_vertex).at(vert) += 1;
	basic_graph::matrix.at(vert).at(number_of_vertex) += 1;
}

void basic_graph::make_union_with_graph(vector<vector<int>>* external_matrix)
{
	int start_count_vert = get_count_vertex();
	int vert = external_matrix->size();
		
	for (int i = 0; i < vert; ++i)
	{
		add_vertex();
	}

	vert += start_count_vert;

	for (int row = start_count_vert, local_row = 0; row < vert; ++row, ++local_row)
	{
		for (int col = start_count_vert, local_col = 0; col < vert; ++col, ++local_col)
		{
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

void basic_graph::BFS(int node_num)
{
	int vert = get_count_vertex();

	if ((node_num < 1) || (node_num > vert))
	{
		cout << "Error: The value is out of range" << endl;
		return;
	}

	queue<int> turn;
	vector<int> used(vert);
	vector<int> path(vert);
	int num_path = 1;

	--node_num;

	path[0] = node_num;
	turn.push(node_num);
	used.at(node_num) = 1;

	while (!turn.empty()) 
	{
		int ind = turn.front();
		turn.pop();
		//cout << (ind+1) << ' ';
		for (int i = 0; i < vert; ++i)
		{
			if ((matrix[ind][i] > 0) && (ind != i) && (used.at(i) == 0))
			{
				turn.push(i);
				path[num_path] = i;
				++num_path;
				used.at(i) = 1;
			}
		}
	}

	for (int i = 0; i < vert; ++i)
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
	cout << path[num_path - 1] + 1 << endl;
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
		if((matrix.at(node_num).at(i) > 0) && (node_num != i) && (used[i] == 0))
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

int basic_graph::floyd_alg()
{
	int vert = get_count_vertex();
	int max_val = 0;

	vector< vector< int>> temp_matrix(vert);
	for (int row = 0; row  < vert; ++row)
	{
		for (int col = 0; col < vert; ++col)
		{
			temp_matrix.at(row).at(col) = matrix.at(row).at(col);
		}
	}

	for (int k = 0; k < vert; ++k) {
		for (int i = 0; i < vert; ++i) {
			for (int j = 0; j < vert; ++j) {
				if (i != j && temp_matrix.at(i).at(k) != 0 && temp_matrix.at(k).at(j) != 0) {
					if (temp_matrix.at(i).at(j) == 0) {
						temp_matrix.at(i).at(j) = temp_matrix.at(i).at(k) + temp_matrix.at(k).at(j);
					}
					else
					{
						temp_matrix.at(i).at(j) = min(temp_matrix.at(i).at(j), temp_matrix.at(i).at(k) + temp_matrix.at(k).at(j));
					}
				}
			}
		}
	}
	for (int i = 0; i < vert; ++i)
	{
		for (int j = 0; j < vert; ++j)
		{
			if (temp_matrix.at(i).at(j) > max_val) max_val = temp_matrix.at(i).at(j);
		}
	}
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
		if ((matrix.at(node_num).at(i) > 0) && (node_num != i))
		{
			if (DFS_for_topological(i, color, stack_vert) == true)
				return true;
		}
	}

	stack_vert->push(node_num);
	color[node_num] = 2;
	return false;
}

bool basic_graph::topological_sort()
{
	int vert = get_count_vertex();
	bool Cycle;
	stack<int> stack_vert;
	int *color = new int[vert];

	for (int i = 0; i < vert; ++i)
	{
	    Cycle = DFS_for_topological(i, color, &stack_vert);
		if (Cycle)
		{
			delete[] color;
			return false;
		}
	}
	for (int i = 0; i < vert; ++i)
	{
		cout << i << ") " << stack_vert.top() << endl;
		stack_vert.pop();
	}

	delete[] color;
	return true;
}